#include "pico/stdlib.h"

//set BUTTON_PIN to GPIO6 (physical pin 9 on the board)
#define BUTTON_PIN 6

int main(void) {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  
    //setup the LED_PIN as an output
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT); 
    gpio_put(LED_PIN, false);

    //setup the BUTTON_PIN as an input
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // enables internal resistor towards 3.3v, pressing button shorts it with the ground.

    //While the buttons logic is high (3.3v) LED is off, when the button logic is low (button pressed) LED is powered on
    while (true) {
        bool pressed = !gpio_get(BUTTON_PIN);
        gpio_put(LED_PIN, pressed);
        sleep_ms(1);
    }
}
