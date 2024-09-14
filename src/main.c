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
    EEPROM eeprom;
    eeprom.pio = pio0;
    eeprom.statemachineId = 0;
    eeprom.gpio = 14;
    eepromInitDriver(&eeprom);

    // EEPROMのリセット
    if (!eepromResetDevice(&eeprom)) {
        printf("Failed to reset device.\n");
        return 1;
    }

    // チップ情報の取得
    sleep_us(150);  // t_htss
    eepromSend(&eeprom, 0xC1);
    uint32_t makerId = 0x00000000;
    for (size_t i = 0; i < 3; i++) {
        uint8_t data = 0x00;
        eepromReceive(&eeprom, &data, i < 2);
        makerId |= data << ((2 - i) * 8);
    }
    sleep_us(150);  // t_htss

    // 結果を表示
    printf("The maker-id of device: %06X\n", makerId);
    while (true) {
        sleep_ms(1000);
    }
}
