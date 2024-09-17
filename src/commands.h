#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

/** EEPROM Access */
static const uint8_t CommandAccessMainMemory = 0x0A;

/** Security Register Access */
static const uint8_t CommandAccessSecurityRegister = 0x0B;

/** Lock Security Register */
static const uint8_t CommandLockSecurityRegister = 0x02;

/** ROM Zone Register Access */
static const uint8_t CommandAccessRomZone = 0x07;

/** Freeze ROM Zone State */
static const uint8_t CommandFreezeRomZoneState = 0x01;

/** Manufacturer ID Read */
static const uint8_t CommandReadManufacturerId = 0x0C;

/** Standard Speed Mode */
static const uint8_t CommandSwitchStandardSpeedMode = 0x0D;

/** High-Speed Mode */
static const uint8_t CommandSwitchHighSpeedMode = 0x0E;

/**
 * @brief コマンド,デバイスアドレス,R/Wからペイロードを生成
 *
 * @param command
 * @param deviceAddress
 * @param isRead
 * @return 作成されたペイロード
 */
static inline uint8_t buildPayload(uint8_t command, uint8_t deviceAddress, bool isRead) {
    return command << 4 | (deviceAddress << 3) | isRead;
}

#endif /* COMMANDS_H */
