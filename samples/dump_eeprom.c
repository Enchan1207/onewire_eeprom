//
// Sample program: dump all stored data
//
#include <ctype.h>
#include <hardware/pio.h>
#include <pico/stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "swi_eeprom/eeprom.h"

/** ビット列を反転する */
uint8_t reversed(uint8_t n) {
    uint8_t source = n;
    uint8_t result = 0;
    for (int i = 7; i >= 0; i--) {
        result <<= 1;
        result |= source & 1;
        source >>= 1;
    }
    return result;
}

/** シリアルナンバーのCRCを照合する */
bool validateSerialNumber(uint64_t value, uint8_t expected) {
    const uint8_t polynomial = 0x31;  // CRC-8 Maxim
    uint8_t result = 0;
    for (int i = 48; i >= 0; i -= 8) {
        uint8_t byte = reversed((value >> i) & 0xFF);
        result ^= byte;

        for (int j = 0; j < 8; ++j) {
            if (result & 0x80) {
                result = (result << 1) ^ polynomial;
            } else {
                result <<= 1;
            }
        }
    }
    return reversed(result) == expected;
}

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

    // シリアル番号の取得
    uint64_t serialNumber = 0;
    if (!eepromQuerySerialNumber(eeprom, &serialNumber)) {
        printf("Failed to query serial number\n");
        return 1;
    }
    bool isValidSerial = validateSerialNumber(serialNumber >> 8, serialNumber & 0xFF);
    printf("Serial number: %016llX (CRC: %s)\n", serialNumber, isValidSerial ? "PASS" : "FAIL");

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
