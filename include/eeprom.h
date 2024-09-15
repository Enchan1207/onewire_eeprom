//
// EEPROM
//
#ifndef EEPROM_H
#define EEPROM_H

#include <hardware/pio.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief EEPROMインスタンス
 */
typedef struct {
    PIO pio;
    uint statemachineId;
    uint gpio;
} EEPROM;

/**
 * @brief EEPROMデバイスドライバを初期化する
 *
 * @param eeprom
 */
void eepromInitDriver(const EEPROM* eeprom);

/**
 * @brief EEPROMデバイスをリセットする
 *
 * @param eeprom
 * @return デバイスの応答
 */
bool eepromResetDevice(const EEPROM* eeprom);

/**
 * @brief EEPROMデバイスを検索する
 *
 * @param eeprom
 * @return 検索できた場合はそのスレーブアドレスが、見つからなかった場合は-1が返ります。
 */
int eepromSearchDevice(const EEPROM* eeprom);

/**
 * @brief EEPROMのメーカIDを取得する
 *
 * @param eeprom
 * @param id
 * @return 取得に失敗した場合はfalseが返ります。
 */
bool eepromQueryMakerId(const EEPROM* eeprom, uint32_t* id);

/**
 * @brief アクセス先のアドレスを指定する
 *
 * @param eeprom
 * @param address
 * @return 指定に失敗した場合はfalseが返ります。
 */
bool eepromSetAddress(const EEPROM* eeprom, uint8_t address);

/**
 * @brief アドレスと値を指定して書き込み
 *
 * @param eeprom
 * @param address
 * @param data
 * @return 書き込みに失敗した場合はfalseが返ります。
 */
bool eepromWriteByte(const EEPROM* eeprom, uint8_t address, uint8_t data);

/**
 * @brief アドレスを指定してランダム読み出し
 *
 * @param eeprom
 * @param address
 * @param value
 * @return アクセスに失敗した場合はfalseが返ります。
 */
bool eepromReadRandom(const EEPROM* eeprom, uint8_t address, uint8_t* value);

#endif /* EEPROM_H */
