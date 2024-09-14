// Single-Wire EEPROM AT21CS01 sample
#include <hardware/clocks.h>
#include <hardware/pio.h>
#include <pico/stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "eeprom.pio.h"

const unsigned int IO_PIN = 14;

const uint8_t CommandManufacturerIDRead = 0x0C;

/**
 * @brief 接続されているデバイスをリセットする
 *
 * @return デバイスが応答したかどうか
 */
bool resetDevice(PIO, uint);

/**
 * @brief デバイスにコマンドを送信する
 *
 * @return ACK
 */
bool sendCommand(PIO, uint, uint8_t);

/**
 * @brief デバイスを検索する
 *
 * @return int 発見できたデバイスのスレーブアドレス (-1: 見つからない)
 */
int searchDevice(PIO, uint);

int main() {
    stdio_init_all();

    const uint statemachineId = 0;
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &singlewireComm_program);

    // PIOアセンブリで定義した関数
    singlewireComm_program_init(pio, statemachineId, offset, IO_PIN);
    pio_sm_set_enabled(pio, statemachineId, true);

    // リセット要求を送り、結果を待つ
    bool resetResult = resetDevice(pio, statemachineId);
    if (!resetResult) {
        printf("Failed to reset device.\n");
        return 1;
    }
    printf("EEPROM Reset successfully.\n");

    // 接続されているデバイスを検索
    printf("Looking for connected devices...\n");
    int deviceAddress = searchDevice(pio, statemachineId);
    if (deviceAddress < 0) {
        printf("No device found\n");
        return 1;
    }
    printf("Device found at address %d.\n", deviceAddress);

    while (true) {
        sleep_ms(1000);
    }
}

bool resetDevice(PIO pio, uint statemachineId) {
    pio_sm_put_blocking(pio, statemachineId, 0xFFFFFFFF);
    return pio_sm_get_blocking(pio, statemachineId) == 0;
}

bool sendCommand(PIO pio, uint statemachineId, uint8_t command) {
    pio_sm_put_blocking(pio, statemachineId, command);
    return pio_sm_get_blocking(pio, statemachineId) == 0;
}

int searchDevice(PIO pio, uint statemachineId) {
    int deviceAddress = -1;
    for (size_t i = 0; i < 8; i++) {
        sleep_us(150);  // t_htss
        uint32_t payload = (CommandManufacturerIDRead << 4) | (i << 1) | 1;
        bool response = sendCommand(pio, statemachineId, payload);
        sleep_us(150);  // t_htss

        if (response) {
            deviceAddress = i;
            break;
        }
    }
    return deviceAddress;
}
