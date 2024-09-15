//
// EEPROMドライバ
//
#ifndef EEPROM_DRIVER_H
#define EEPROM_DRIVER_H

#include <hardware/pio.h>
#include <stdbool.h>

#include "eeprom.h"

/**
 * @brief EEPROMと通信するPIOステートマシンを初期化する
 *
 * @param eeprom
 */
void eepromInitStateMachine(const EEPROM* eeprom);

/**
 * @brief EEPROMデバイスをリセットする
 *
 * @param eeprom
 * @return デバイスの応答
 */
bool eepromReset(const EEPROM* eeprom);

/**
 * @brief EEPROMデバイスにコマンドを送信する
 *
 * @param eeprom
 * @param command
 * @return 送信結果
 */
bool eepromSend(const EEPROM* eeprom, uint8_t command);

/**
 * @brief EEPROMデバイスから応答を受信する
 *
 * @param eeprom
 * @param data
 * @param keepAlive falseに設定すると、データの受信を停止し、NACKをデバイスに送信します。
 */
void eepromReceive(const EEPROM* eeprom, uint8_t* data, bool keepAlive);

/**
 * @brief EEPROMデバイスから複数バイトの応答を受信する
 *
 * @param eeprom
 * @param data
 * @param length 受け取るデータの長さ
 *
 * @note 格納先(`data`)には長さ(`length`)分以上の領域が確保されていることを前提とします。
 */
void eepromReceiveArray(const EEPROM* eeprom, uint8_t* data, uint8_t length);

#endif /* EEPROM_DRIVER_H */
