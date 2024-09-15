//
// Single-Wire EEPROM AT21CS01 を触ってみる
//
#include <hardware/pio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "eeprom.h"

int main() {
    stdio_init_all();

    // ドライバの準備と初期化
    EEPROM er, *eeprom;
    er.pio = pio0;
    er.statemachineId = 0;
    er.gpio = 14;
    eeprom = &er;
    eepromInitDriver(eeprom);

    // EEPROMのリセット
    if (!eepromResetDevice(eeprom)) {
        printf("Failed to reset device.\n");
        return 1;
    }

    // メーカーIDの取得
    uint32_t makerId = 0;
    if (!eepromQueryMakerId(eeprom, &makerId)) {
        printf("Failed to query maker ID\n");
        return 1;
    }
    printf("Maker id of connected device: %06X\n", makerId);

    // ランダムリード
    printf("Try to read data from first address\n");
    uint8_t address = 0x00;
    uint8_t data = 0;
    if (!eepromReadRandom(eeprom, address, &data)) {
        printf("Failed to read.\n");
        return 1;
    }
    printf("Stored data @ %02X: %02X\n", address, data);

    // シングルライト
    uint8_t writeData = data ^ 0x55;
    printf("Try to write %02X to %02X\n", writeData, address);
    if (!eepromWriteByte(eeprom, address, writeData)) {
        printf("Failed to write.\n");
        return 1;
    }

    printf("Finished.\n");

    while (true) {
        sleep_ms(1000);
    }
}
