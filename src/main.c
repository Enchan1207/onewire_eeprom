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

    // メーカーIDの取得
    uint8_t makerId[3] = {0};
    sleep_us(150);  // t_htss
    eepromSend(&eeprom, 0xC1);
    eepromReceiveArray(&eeprom, makerId, 3);
    printf("The maker-id of device: ", makerId);
    for (size_t i = 0; i < 3; i++) {
        printf("%02X", makerId[i]);
    }
    printf("\n");

    sleep_us(150);  // t_htss

    // 結果を表示
    while (true) {
        sleep_ms(1000);
    }
}
