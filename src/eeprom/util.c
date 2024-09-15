#include <pico/stdlib.h>

#include "commands.h"
#include "driver.h"
#include "eeprom.h"

void eepromInitDriver(const EEPROM* eeprom) {
    eepromInitStateMachine(eeprom);
}

bool eepromResetDevice(const EEPROM* eeprom) {
    return eepromReset(eeprom);
}

int eepromSearchDevice(const EEPROM* eeprom) {
    int deviceAddress = -1;
    for (uint8_t i = 0; i < 8; i++) {
        sleep_us(150);  // t_htss
        bool response = eepromSend(eeprom, buildPayload(CommandReadManufacturerId, eeprom->deviceAddress, true));
        sleep_us(150);  // t_htss

        if (response) {
            deviceAddress = i;
            break;
        }
    }
    return deviceAddress;
}

bool eepromQueryMakerId(const EEPROM* eeprom, uint32_t* id) {
    uint32_t queriedId = 0;
    sleep_us(150);  // t_htss
    if (!eepromSend(eeprom, buildPayload(CommandReadManufacturerId, eeprom->deviceAddress, true))) {
        sleep_us(150);
        return false;
    }

    uint8_t data = 0;
    for (size_t i = 0; i < 3; i++) {
        eepromReceive(eeprom, &data, i < 2);
        queriedId = queriedId << 8 | data;
    }
    sleep_us(150);  // t_htss
    *id = queriedId;
    return true;
}

static bool setAddress(const EEPROM* eeprom, uint8_t address) {
    if (!eepromSend(eeprom, buildPayload(CommandAccessMainMemory, eeprom->deviceAddress, false))) {
        return false;
    }
    if (!eepromSend(eeprom, address)) {
        return false;
    }
    return true;
}

bool eepromWriteByte(const EEPROM* eeprom, uint8_t address, uint8_t data) {
    sleep_us(150);

    // 書き込み先を指定
    if (!setAddress(eeprom, address)) {
        sleep_us(150);
        return false;
    }

    // 書き込み
    if (!eepromSend(eeprom, data)) {
        sleep_us(150);
        return false;
    }

    sleep_us(150);
    return true;
}

size_t eepromWritePage(const EEPROM* eeprom, uint8_t page, const uint8_t* data, size_t length) {
    sleep_us(150);

    // 書き込み先を指定
    if (!setAddress(eeprom, page << 3)) {
        sleep_us(150);
        return 0;
    }

    // 書き込み
    size_t writtenBytes = 0;
    while (writtenBytes < length) {
        // 途中で失敗したら抜ける
        if (!eepromSend(eeprom, data[writtenBytes])) {
            break;
        }
        writtenBytes++;
    }

    sleep_us(150);
    return writtenBytes;
}

bool eepromReadRandom(const EEPROM* eeprom, uint8_t address, uint8_t* value) {
    sleep_us(150);

    // 読み出し元を指定
    if (!setAddress(eeprom, address)) {
        sleep_us(150);
        return false;
    }

    sleep_us(150);

    // 読み出し
    if (!eepromSend(eeprom, buildPayload(CommandAccessMainMemory, eeprom->deviceAddress, true))) {
        sleep_us(150);
        return false;
    }
    eepromReceive(eeprom, value, false);

    sleep_us(150);
    return true;
}

bool eepromReadSequential(const EEPROM* eeprom, uint8_t address, uint8_t* data, size_t length) {
    sleep_us(150);

    // 読み出し元を指定
    if (!setAddress(eeprom, address)) {
        sleep_us(150);
        return false;
    }

    sleep_us(150);

    // 読み出し
    if (!eepromSend(eeprom, buildPayload(CommandAccessMainMemory, eeprom->deviceAddress, true))) {
        sleep_us(150);
        return false;
    }

    // 連続で読み出しを続ける
    size_t readBytes = 0;
    while (readBytes < length) {
        eepromReceive(eeprom, data + readBytes, readBytes < (length - 1));
        readBytes++;
    }

    sleep_us(150);
    return readBytes;
}
