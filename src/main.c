// template: blink LED at pin 22 with input from pin 21
#include <pico/stdlib.h>

const unsigned int LED_PIN = 22;
const unsigned int SWITCH_PIN = 21;

int main() {
    // initialize GPIO
    gpio_init(LED_PIN);
    gpio_init(SWITCH_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);

    // loop
    while (true) {
        bool isPressed = !gpio_get(SWITCH_PIN);
        gpio_put(LED_PIN, isPressed);
    }
}
