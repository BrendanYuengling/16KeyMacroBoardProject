#include "pico/stdlib.h"
#include <stdio.h>

#define ROW0 2
#define ROW1 3
#define ROW2 4
#define ROW3 5
#define COL0 6
#define COL1 7
#define COL2 8
#define COL3 9


int main(void) {
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

    while(true) {
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

        printf("SW1=%d SW2=%d SW3=%d SW4=%d SW5=%d SW6=%d SW7=%d SW8=%d SW9=%d SW10=%d SW11=%d SW12=%d SW13=%d SW14=%d SW15=%d SW16=%d\n",sw1Pressed, sw2Pressed, sw3Pressed, sw4Pressed,sw5Pressed, sw6Pressed, sw7Pressed, sw8Pressed,sw9Pressed, sw10Pressed, sw11Pressed, sw12Pressed,sw13Pressed, sw14Pressed, sw15Pressed, sw16Pressed);     
        sleep_ms(100);
    }
}
