
#include "pico/stdlib.h"

#define BUTTON_PIN 6

int main(void) {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);


//two new variables to allow for the toggleability
    bool lightToggle = false;
    bool previouslyPressed = false;

    while (true) {
        bool pressed = !gpio_get(BUTTON_PIN);

//since we check the button state so rapidly we must check the previous state as well to prevent a rapid switching
        if (pressed == true && previouslyPressed == false) {
            lightToggle = !lightToggle;
        };
        gpio_put(LED_PIN, lightToggle);

//set previous state variable to current state 
        previouslyPressed = pressed;   
        sleep_ms(1);
    }
}
