// Single-Wire EEPROM AT21CS01 sample
#include <hardware/clocks.h>
#include <hardware/pio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "eeprom.pio.h"

const unsigned int IO_PIN = 14;

const uint8_t ACK = 0;
const uint8_t NACK = 1;

const uint8_t CommandManufacturerIDRead = 0x0C;

int main() {
    stdio_init_all();

    const uint statemachineId = 0;
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &singlewireComm_program);

    // PIOアセンブリで定義した関数
    singlewireComm_program_init(pio, statemachineId, offset, IO_PIN);
    pio_sm_set_enabled(pio, statemachineId, true);

    // リセット要求を送り、結果を待つ
    pio_sm_put_blocking(pio, statemachineId, 0xFFFFFFFF);
    uint32_t resetResult = pio_sm_get_blocking(pio, statemachineId);
    if (resetResult != 0) {
        printf("EEPROM Reset failed\n");
        while (true) {
            sleep_ms(1000);
        }
    }
    printf("EEPROM Reset successfully\n");

    // オペコード 0x0C(チップ情報読み取り) を各スレーブアドレスに対して送信し、デバイスを探す
    printf("Looking for connected devices...\n");
    int deviceAddress = -1;
    for (size_t i = 0; i < 8; i++) {
        sleep_us(150);  // t_htss
        uint32_t payload = (CommandManufacturerIDRead << 4) | (i << 1) | 1;
        pio_sm_put_blocking(pio, statemachineId, payload);
        uint32_t response = pio_sm_get_blocking(pio, statemachineId);
        printf("address:%d ack:%d\n", i, response);
        sleep_us(150);  // t_htss

        if (response == ACK) {
            deviceAddress = i;
            break;
        }
    }

    if (deviceAddress < 0) {
        printf("No device found\n");
        while (true) {
            sleep_ms(1000);
        }
    }
    printf("Device found at address %d!\n", deviceAddress);

    while (true) {
        sleep_ms(1000);
    }
}
