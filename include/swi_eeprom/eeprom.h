//
// Single-Wire Interface EEPROM driver
//
#ifndef EEPROM_H
#define EEPROM_H

#include <hardware/pio.h>
#include <stdbool.h>
#include <stdint.h>

/** EEPROMインスタンス */
typedef struct {
    PIO pio;
    uint statemachineId;

    /** 通信に使用するGPIOのピン番号 */
    uint gpio;

    /** EEPROMのデバイスアドレス */
    uint8_t deviceAddress;
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
 * @brief EEPROMのシリアル番号を取得する
 *
 * @param eeprom
 * @param id
 * @return 取得に失敗した場合はfalseが返ります。
 */
bool eepromQuerySerialNumber(const EEPROM* eeprom, uint64_t* id);

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
 * @brief ページ書き込み(最大8byteを同時書き込み)
 *
 * @param eeprom
 * @param page ページ番号(0x00~0x0F)
 * @param data 書き込みデータへのポインタ
 * @param length dataの長さ (最大8)
 * @return 書き込めたバイト数
 */
size_t eepromWritePage(const EEPROM* eeprom, uint8_t page, const uint8_t* data, size_t length);

/**
 * @brief アドレスを指定してランダム読み出し
 *
 * @param eeprom
 * @param address
 * @param value
 * @return アクセスに失敗した場合はfalseが返ります。
 */
bool eepromReadRandom(const EEPROM* eeprom, uint8_t address, uint8_t* value);

/**
 * @brief アドレスを指定してシーケンシャル読み出し
 *
 * @param eeprom
 * @param address
 * @param data 格納先
 * @param length 読み出すデータ長
 * @return アクセスに失敗した場合はfalseが返ります。
 */
bool eepromReadSequential(const EEPROM* eeprom, uint8_t address, uint8_t* data, size_t length);

#endif /* EEPROM_H */
