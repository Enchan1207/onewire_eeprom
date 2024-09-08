// Single-Wire EEPROM AT21CS01 sample
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

const unsigned int IO_PIN = 14;

int main() {
    stdio_init_all();

    gpio_init(IO_PIN);
    gpio_set_dir(IO_PIN, GPIO_OUT);

    // reset device
    gpio_put(IO_PIN, false);
    sleep_us(50);  // t_reset min@high: 48

    gpio_set_dir(IO_PIN, GPIO_IN);
    sleep_us(10);  // t_rrt min@high: 8

    gpio_set_dir(IO_PIN, GPIO_OUT);
    gpio_put(IO_PIN, false);
    sleep_us(2);  // t_drr max@high: 2 - t_pup
    gpio_set_dir(IO_PIN, GPIO_IN);

    // ACK lasts up to 24us
    uint8_t ackTimes = 30;
    while (--ackTimes > 0 && gpio_get(IO_PIN) == true) {
        // Actually, getting state from gpio requires more than 1us (I think), so this sleep won't be needed
        sleep_us(1);
    }

    // Confirm ACK
    if (ackTimes > 0) {
        printf("ACK from EEPROM received\n");
    } else {
        printf("EEPROM reset timed out\n");
        while (true) {
        }
    }

    // loop
    while (true) {
        sleep_ms(1000);
    }
}
