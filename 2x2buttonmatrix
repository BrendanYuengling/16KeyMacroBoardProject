#include "pico/stdlib.h"
#include <stdio.h>

#define ROW0 2
#define ROW1 3
#define COL0 6
#define COL1 7


int main(void) {
//init COL0
    gpio_init(COL0);
    gpio_set_dir(COL0, GPIO_IN);
    gpio_pull_up(COL0);
//init COL1
    gpio_init(COL1);
    gpio_set_dir(COL1, GPIO_IN);
    gpio_pull_up(COL1);
//init ROW0
    gpio_init(ROW0);
    gpio_set_dir(ROW0, GPIO_IN);
    gpio_disable_pulls(ROW0);
    gpio_put(ROW0, false);
//init ROW1
    gpio_init(ROW1);
    gpio_set_dir(ROW1, GPIO_IN);
    gpio_disable_pulls(ROW1);
    gpio_put(ROW1, false);

    stdio_init_all();   

    while(true) {
    //set row0 active then scan col1 and col0, 
        gpio_set_dir(ROW0, GPIO_OUT);
        sleep_us(10);       
        bool sw1Pressed = !gpio_get(COL0);
        bool sw2Pressed = !gpio_get(COL1);
        gpio_set_dir(ROW0, GPIO_IN);
        //set row0 active then scan col1 and col0, 
        gpio_set_dir(ROW1, GPIO_OUT);
        sleep_us(10);       
        bool sw3Pressed = !gpio_get(COL0);
        bool sw4Pressed = !gpio_get(COL1);
        gpio_set_dir(ROW1, GPIO_IN);

  //output active switches. 
  //to view use serial monitor extension (Made by microsoft) in VSC
    //if needed use device manager to view port
        printf("SW1=%d SW2=%d SW3=%d SW4=%d\n",sw1Pressed, sw2Pressed, sw3Pressed, sw4Pressed);     
        sleep_ms(100);
    }
}
