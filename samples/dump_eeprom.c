//
// Sample program: dump all stored data
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

    // ページごとにメモリをダンプ
    printf("Dump stored data...\n");
    const uint8_t pageSize = 8;
    const size_t memoryBitsCapacity = 1024;
    const size_t nPages = memoryBitsCapacity / 8 / pageSize;
    for (size_t pageIndex = 0; pageIndex < nPages; pageIndex++) {
        uint8_t startAddress = pageIndex * pageSize;
        uint8_t page[pageSize];
        if (!eepromReadSequential(eeprom, startAddress, page, pageSize)) {
            printf("\n\n***Failed to read from address %02X. aborting\n", startAddress);
            break;
        }
        printf("%02Xh: ", startAddress);

        // hex dump
        for (size_t i = 0; i < pageSize; i++) {
            printf("%02X", page[i]);
        }

        printf(" | ");

        // ascii dump
        for (size_t i = 0; i < pageSize; i++) {
            if (!isprint(page[i])) {
                printf(".");
                continue;
            }
            printf("%c", page[i]);
        }

        printf("\n");
    }
    return 0;
}
