//
// ユーティリティ
//
#include <pico/stdlib.h>

#include "eeprom.h"
#include "eeprom_driver.h"

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
        uint32_t payload = (0x0C << 4) | (i << 1) | 1;
        bool response = eepromSend(eeprom, payload);
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
    if (!eepromSend(eeprom, 0xC1)) {
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
    if (!eepromSend(eeprom, 0xA0)) {
        return false;
    }
    if (!eepromSend(eeprom, address)) {
        return false;
    }
    return true;
}

bool eepromSetAddress(const EEPROM* eeprom, uint8_t address) {
    sleep_us(150);
    bool result = setAddress(eeprom, address);
    sleep_us(150);
    return result;
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

bool eepromReadRandom(const EEPROM* eeprom, uint8_t address, uint8_t* value) {
    sleep_us(150);

    // 読み出し元を指定
    if (!setAddress(eeprom, address)) {
        sleep_us(150);
        return false;
    }

    // 読み出し
    if (!eepromSend(eeprom, 0xA1)) {
        sleep_us(150);
        return false;
    }
    eepromReceive(eeprom, value, false);

    sleep_us(150);
    return true;
}
