//
// Sample program: write dummy content
//
#include <ctype.h>
#include <hardware/pio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "swi_eeprom/eeprom.h"

int main() {
    stdio_init_all();

    // ドライバの準備と初期化
    EEPROM er, *eeprom;
    er.pio = pio0;
    er.statemachineId = 0;
    er.gpio = 14;
    er.deviceAddress = 0;
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
    printf("Maker ID: %06X\n", makerId);

    // データ書き込み
    printf("Write dummy data...\n");
    const char string[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
    for (size_t i = 0; i < sizeof(string); i++) {
        eepromWriteByte(eeprom, i, string[i]);
    }

    printf("Finished.\n");
    return 0;
}
