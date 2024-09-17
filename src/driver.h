//
// EEPROMドライバ
//
#ifndef EEPROM_DRIVER_H
#define EEPROM_DRIVER_H

#include <hardware/pio.h>
#include <stdbool.h>

#include "swi_eeprom/eeprom.h"

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
 * @brief EEPROMデバイスにペイロードを送信する
 *
 * @param eeprom
 * @param payload
 * @return 送信結果
 */
bool eepromSend(const EEPROM* eeprom, uint8_t payload);

/**
 * @brief EEPROMデバイスから応答を受信する
 *
 * @param eeprom
 * @param data
 * @param keepAlive falseに設定すると、データの受信を停止し、NACKをデバイスに送信します。
 */
void eepromReceive(const EEPROM* eeprom, uint8_t* data, bool keepAlive);

#endif /* EEPROM_DRIVER_H */
