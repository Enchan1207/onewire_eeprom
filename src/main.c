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
bool send(PIO, uint, uint8_t);

/**
 * @brief デバイスからレスポンスを受信する
 */
void receive(PIO, uint, uint8_t*);

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
    bool isReset = resetDevice(pio, statemachineId);
    printf("Device reset result: %s\n", isReset ? "Success" : "Failure");
    if (!isReset) {
        printf("Failed to reset device.\n");
        return 1;
    }

    sleep_us(150);  // t_htss

    // チップ情報を取得するコマンドを送信
    bool result = send(pio, statemachineId, 0xC1);
    if (!result) {
        printf("Failed to send command.\n");
        return 1;
    }

    // 3byteのメーカコードを受信
    uint32_t makerId = 0x00000000;
    for (size_t i = 0; i < 3; i++) {
        uint8_t data = 0x00;
        receive(pio, statemachineId, &data);
        makerId |= data << ((2 - i) * 8);
    }
    // TODO: 本当はここでNACKしないといけない！

    sleep_us(150);  // t_htss

    // 結果を表示
    printf("The maker-id of device: %06X\n", makerId);
    while (true) {
        sleep_ms(1000);
    }
}

bool resetDevice(PIO pio, uint statemachineId) {
    pio_sm_put_blocking(pio, statemachineId, 0xFFFFFFFF);
    return pio_sm_get_blocking(pio, statemachineId) == 0;
}

bool send(PIO pio, uint statemachineId, uint8_t command) {
    pio_sm_put_blocking(pio, statemachineId, command);
    uint32_t result = pio_sm_get_blocking(pio, statemachineId);
    return (result & 0x01) == 0;
}

void receive(PIO pio, uint statemachineId, uint8_t* data) {
    pio_sm_put_blocking(pio, statemachineId, 0x00FFFFFF);
    uint32_t result = pio_sm_get_blocking(pio, statemachineId);
    *data = result & 0xFF;
}

int searchDevice(PIO pio, uint statemachineId) {
    int deviceAddress = -1;
    for (size_t i = 0; i < 8; i++) {
        sleep_us(150);  // t_htss
        uint32_t payload = (CommandManufacturerIDRead << 4) | (i << 1) | 1;
        bool response = send(pio, statemachineId, payload);
        sleep_us(150);  // t_htss

        if (response) {
            deviceAddress = i;
            break;
        }
    }
    return deviceAddress;
}
