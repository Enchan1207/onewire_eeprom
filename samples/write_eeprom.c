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
    printf("Write dummy data");
    const uint8_t pageSize = 8;
    const char string[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
    for (size_t i = 0; i < sizeof(string); i += pageSize) {
        // 効率化のため8byte=1pageずつ書き込む
        const uint8_t pageIndex = i >> 3;
        if (!eepromWritePage(eeprom, pageIndex, (const uint8_t*)(string + i), pageSize)) {
            printf("\nFailed to write at page %02X.\n", pageIndex);
            break;
        }

        // 書き込みサイクル時間待機
        printf(".");
        sleep_ms(5);
    }
    printf("\n");

    printf("Finished.\n");
    return 0;
}
