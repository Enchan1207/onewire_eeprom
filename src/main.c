// Single-Wire EEPROM AT21CS01 sample
#include <hardware/clocks.h>
#include <hardware/pio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "eeprom.pio.h"

const unsigned int IO_PIN = 14;

int main() {
    stdio_init_all();

    const uint statemachineId = 0;

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &singlewireComm_program);

    // PIOアセンブリで定義した関数
    singlewireComm_program_init(pio, statemachineId, offset, IO_PIN);
    pio_sm_set_enabled(pio, statemachineId, true);

    // スタックに積んだ瞬間に動き出す
    pio_sm_put_blocking(pio, statemachineId, 0x5A);

    while (true) {
        sleep_ms(1000);
    }
}
