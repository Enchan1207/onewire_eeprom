//
// Single-Wire EEPROM AT21CS01 を触ってみる
//
#include <hardware/pio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "eeprom.h"

int main() {
    stdio_init_all();

    // ドライバの準備と初期化
    EEPROM er, *eeprom;
    er.pio = pio0;
    er.statemachineId = 0;
    er.gpio = 14;
    eeprom = &er;
    eepromInitDriver(eeprom);

    // EEPROMのリセット
    if (!eepromResetDevice(eeprom)) {
        printf("Failed to reset device.\n");
        return 1;
    }

    // メーカーIDの取得
    uint8_t makerId[3] = {0};
    sleep_us(150);  // t_htss
    eepromSend(eeprom, 0xC1);
    eepromReceiveArray(eeprom, makerId, 3);
    printf("The maker-id of device: ", makerId);
    for (size_t i = 0; i < 3; i++) {
        printf("%02X", makerId[i]);
    }
    printf("\n");
    sleep_us(150);  // t_htss

    // 書き込みアドレス指定
    sleep_us(150);  // t_htss
    eepromSend(eeprom, 0xA0);
    if (!eepromSend(eeprom, 0x00)) {
        printf("Failed to specify address.\n");
        return 1;
    }

    // 値を書き込む
    if (!eepromSend(eeprom, 0xC7)) {
        printf("Failed to write.\n");
        return 1;
    }
    sleep_us(150);  // t_htss

    // 読み出しアドレス指定
    sleep_us(150);  // t_htss
    eepromSend(eeprom, 0xA0);
    eepromSend(eeprom, 0x00);
    eepromSend(eeprom, 0xA1);
    uint8_t storedData = 0x00;
    eepromReceive(eeprom, &storedData, false);
    sleep_us(150);  // t_htss

    printf("Stored data: %02X\n", storedData);

    // 結果を表示
    while (true) {
        sleep_ms(1000);
    }
}
