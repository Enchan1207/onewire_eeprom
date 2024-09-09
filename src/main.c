// Single-Wire EEPROM AT21CS01 sample
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

const unsigned int IO_PIN = 14;

bool eeprom_reset() {
    gpio_set_dir(IO_PIN, GPIO_OUT);
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
    return ackTimes > 0;
}

void eeprom_write_bit(bool value) {
    // TODO: もう少し時間縮められるはず
    const uint8_t tBit = 20;              // t_bit max@high: 25
    const uint8_t tLow = value ? 1 : 10;  // t_low0 max@high: 16 t_low1 max@high: 2
    gpio_set_dir(IO_PIN, GPIO_OUT);
    gpio_put(IO_PIN, false);
    sleep_us(tLow);
    gpio_set_dir(IO_PIN, GPIO_IN);
    sleep_us(tBit - tLow);
}

void eeprom_write_byte(uint8_t data) {
    // MSB first
    for (size_t i = 0; i < 8; i++) {
        eeprom_write_bit(data & 1 << (7 - i));
    }
}

bool eeprom_read_bit() {
    const uint8_t tBit = 20;  // t_bit max@high: 25
    const uint8_t tRd = 1;    // t_rd max@high: 2 - t_pup
    const uint8_t tMrs = 2;   // t_mrs max@high: 2
    gpio_set_dir(IO_PIN, GPIO_OUT);
    gpio_put(IO_PIN, false);
    sleep_us(tRd);
    gpio_set_dir(IO_PIN, GPIO_IN);
    sleep_us(tMrs - tRd);
    bool result = gpio_get(IO_PIN);
    sleep_us(tBit - tRd);
    return result;
}

int main() {
    stdio_init_all();

    gpio_init(IO_PIN);
    gpio_disable_pulls(IO_PIN);

    // Reset EEPROM
    bool detected = eeprom_reset();
    if (detected) {
        printf("EEPROM successfully reset.\n");
    } else {
        printf("EEPROM reset timed out.\n");
    }

    // send command _Manufacturer ID Read_ (0x0C)
    const uint8_t opCode = 0x0C;

    // The slave address (A2-A0) cannot be confirmed unless SOIC or XSFN!

    // start condition
    gpio_set_dir(IO_PIN, GPIO_IN);
    sleep_us(160);  // t_htss min@high: 150

    const uint8_t deviceAddress = 0x00;

    // <opcode><address><r/w#>
    const uint8_t command = (opCode << 4) | ((deviceAddress & 0x07) << 1) | 0x01;
    printf("Try to send %02X... ", command);
    eeprom_write_byte(command);
    printf("Sent.\n");
    bool ack = eeprom_read_bit();
    if (!ack) {
        printf("Device reacted command. (attempted device address: %d)\n", deviceAddress);
    } else {
        printf("No response. (attempted device address: %d)\n", deviceAddress);
    }

    // loop
    while (true) {
        sleep_ms(1000);
    }
}
