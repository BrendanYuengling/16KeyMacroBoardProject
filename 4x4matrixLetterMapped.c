/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"


#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"

#define ROW0 2
#define ROW1 3
#define ROW2 4
#define ROW3 5
#define COL0 6
#define COL1 7
#define COL2 8
#define COL3 9

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);
void hid_task(void);

/*------------- MAIN -------------*/
int main(void)
{
    gpio_init(COL0);
    gpio_set_dir(COL0, GPIO_IN);
    gpio_pull_up(COL0);

    gpio_init(COL1);
    gpio_set_dir(COL1, GPIO_IN);
    gpio_pull_up(COL1);

    gpio_init(COL2);
    gpio_set_dir(COL2, GPIO_IN);
    gpio_pull_up(COL2);

    gpio_init(COL3);
    gpio_set_dir(COL3, GPIO_IN);
    gpio_pull_up(COL3);

    gpio_init(ROW0);
    gpio_set_dir(ROW0, GPIO_IN);
    gpio_disable_pulls(ROW0);
    gpio_put(ROW0, false);

    gpio_init(ROW1);
    gpio_set_dir(ROW1, GPIO_IN);
    gpio_disable_pulls(ROW1);
    gpio_put(ROW1, false);

    gpio_init(ROW2);
    gpio_set_dir(ROW2, GPIO_IN);
    gpio_disable_pulls(ROW2);
    gpio_put(ROW2, false);

    gpio_init(ROW3);
    gpio_set_dir(ROW3, GPIO_IN);
    gpio_disable_pulls(ROW3);
    gpio_put(ROW3, false);

    stdio_init_all();   

  board_init();

  // init device stack on configured roothub port
  const tusb_rhport_init_t rh_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUD_OPT_HIGH_SPEED ? TUSB_SPEED_HIGH : TUSB_SPEED_FULL
  };
  TU_ASSERT(tud_rhport_init(BOARD_TUD_RHPORT, &rh_init));
  board_init_after_tusb();

  while (1)
  {
    tud_task(); // tinyusb device task
    led_blinking_task();

    hid_task();
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

static void send_hid_report(uint8_t report_id, uint32_t btn)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

  switch(report_id)
  {
    case REPORT_ID_KEYBOARD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_keyboard_key = false;

      if ( btn )
      {
        uint8_t keycode[6] = { 0 };
        keycode[0] = HID_KEY_A;

        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
        has_keyboard_key = true;
      }else
      {
        // send empty key report if previously has key pressed
        if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_keyboard_key = false;
      }
    }
    break;

    case REPORT_ID_MOUSE:
    {
      int8_t const delta = 5;

      // no button, right + down, no scroll, no pan
      tud_hid_mouse_report(REPORT_ID_MOUSE, 0x00, delta, delta, 0, 0);
    }
    break;

    case REPORT_ID_CONSUMER_CONTROL:
    {
      // use to avoid send multiple consecutive zero report
      static bool has_consumer_key = false;

      if ( btn )
      {
        // volume down
        uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
        has_consumer_key = true;
      }else
      {
        // send empty key report (release key) if previously has key pressed
        uint16_t empty_key = 0;
        if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);
        has_consumer_key = false;
      }
    }
    break;

    case REPORT_ID_GAMEPAD:
    {
      // use to avoid send multiple consecutive zero report for keyboard
      static bool has_gamepad_key = false;

      hid_gamepad_report_t report =
      {
        .x   = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0,
        .hat = 0, .buttons = 0
      };

      if ( btn )
      {
        report.hat = GAMEPAD_HAT_UP;
        report.buttons = GAMEPAD_BUTTON_A;
        tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));

        has_gamepad_key = true;
      }else
      {
        report.hat = GAMEPAD_HAT_CENTERED;
        report.buttons = 0;
        if (has_gamepad_key) tud_hid_report(REPORT_ID_GAMEPAD, &report, sizeof(report));
        has_gamepad_key = false;
      }
    }
    break;

    default: break;
  }
}

#if TUSB_VERSION_NUMBER > 1800
// board_millis has been removed from tinyusb. Use tusb_time_millis_api instead
#define board_millis tusb_time_millis_api
#endif

// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete
void hid_task(void)
{
  // Poll every 10ms
  const uint32_t interval_ms = 10;
  static uint32_t start_ms = 0;

  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  gpio_set_dir(ROW0, GPIO_OUT);
  sleep_us(10);       
  bool sw1Pressed = !gpio_get(COL0);
  bool sw2Pressed = !gpio_get(COL1);
  bool sw3Pressed = !gpio_get(COL2);
  bool sw4Pressed = !gpio_get(COL3);
  gpio_set_dir(ROW0, GPIO_IN);

  gpio_set_dir(ROW1, GPIO_OUT);
  sleep_us(10);       
  bool sw5Pressed = !gpio_get(COL0);
  bool sw6Pressed = !gpio_get(COL1);
  bool sw7Pressed = !gpio_get(COL2);
  bool sw8Pressed = !gpio_get(COL3);
  gpio_set_dir(ROW1, GPIO_IN);

  gpio_set_dir(ROW2, GPIO_OUT);
  sleep_us(10);       
  bool sw9Pressed = !gpio_get(COL0);
  bool sw10Pressed = !gpio_get(COL1);
  bool sw11Pressed = !gpio_get(COL2);
  bool sw12Pressed = !gpio_get(COL3);
  gpio_set_dir(ROW2, GPIO_IN);

  gpio_set_dir(ROW3, GPIO_OUT);
  sleep_us(10);       
  bool sw13Pressed = !gpio_get(COL0);
  bool sw14Pressed = !gpio_get(COL1);
  bool sw15Pressed = !gpio_get(COL2);
  bool sw16Pressed = !gpio_get(COL3);
  gpio_set_dir(ROW3, GPIO_IN);


  //Switch definitions
  bool pressed[16] = {sw1Pressed, sw2Pressed, sw3Pressed, sw4Pressed,sw5Pressed, sw6Pressed, sw7Pressed, sw8Pressed,sw9Pressed, sw10Pressed, sw11Pressed, sw12Pressed,sw13Pressed, sw14Pressed, sw15Pressed, sw16Pressed};
  static const uint8_t letterForSwitch[16] = {HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D, HID_KEY_E, HID_KEY_F, HID_KEY_G, HID_KEY_H, HID_KEY_I, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_M, HID_KEY_N, HID_KEY_O, HID_KEY_P};
  uint8_t keycodes[6] = {0};
  int slot = 0;

  for (int i=0 ; i<16 && slot < 6 ; ++i) {
    if (pressed[i]) {
      keycodes[slot] = letterForSwitch[i];
      ++slot;
    }
  }
  if (tud_hid_ready()) {
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycodes);
  }
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;
  (void) report;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

      uint8_t const kbd_leds = buffer[0];

      if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
      {
        // Capslock On: disable blink, turn led on
        blink_interval_ms = 0;
        board_led_write(true);
      }else
      {
        // Caplocks Off: back to normal blink
        board_led_write(false);
        blink_interval_ms = BLINK_MOUNTED;
      }
    }
  }
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // blink is disabled
  if (!blink_interval_ms) return;

  // Blink every interval ms
  if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
