#include "driver.h"

#include <driver.pio.h>
#include <hardware/pio.h>
#include <pico/stdlib.h>

void eepromInitStateMachine(const EEPROM* eeprom) {
    uint offset = pio_add_program(eeprom->pio, &eeprom_program);
    eeprom_program_init(eeprom->pio, eeprom->statemachineId, offset, eeprom->gpio);
    pio_sm_set_enabled(eeprom->pio, eeprom->statemachineId, true);
}

bool eepromReset(const EEPROM* eeprom) {
    pio_sm_put_blocking(eeprom->pio, eeprom->statemachineId, 0xFFFFFFFF);
    return pio_sm_get_blocking(eeprom->pio, eeprom->statemachineId) == 0;
}

bool eepromSend(const EEPROM* eeprom, uint8_t payload) {
    pio_sm_put_blocking(eeprom->pio, eeprom->statemachineId, payload);
    uint32_t result = pio_sm_get_blocking(eeprom->pio, eeprom->statemachineId);
    return (result & 0x01) == 0;
}

void eepromReceive(const EEPROM* eeprom, uint8_t* data, bool keepAlive) {
    pio_sm_put_blocking(eeprom->pio, eeprom->statemachineId, keepAlive ? 0x1FF : 0x2FF);
    uint32_t result = pio_sm_get_blocking(eeprom->pio, eeprom->statemachineId);
    *data = result & 0xFF;
}
