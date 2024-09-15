//
// EEPROMドライバ
//
#include <hardware/pio.h>
#include <pico/stdlib.h>

#include "eeprom.h"
#include "eeprom.pio.h"

const uint8_t CommandManufacturerIDRead = 0x0C;

void eepromInitDriver(const EEPROM* eeprom) {
    uint offset = pio_add_program(eeprom->pio, &eeprom_program);
    eeprom_program_init(eeprom->pio, eeprom->statemachineId, offset, eeprom->gpio);
    pio_sm_set_enabled(eeprom->pio, eeprom->statemachineId, true);
}

bool eepromResetDevice(const EEPROM* eeprom) {
    pio_sm_put_blocking(eeprom->pio, eeprom->statemachineId, 0xFFFFFFFF);
    return pio_sm_get_blocking(eeprom->pio, eeprom->statemachineId) == 0;
}

bool eepromSend(const EEPROM* eeprom, uint8_t command) {
    pio_sm_put_blocking(eeprom->pio, eeprom->statemachineId, command);
    uint32_t result = pio_sm_get_blocking(eeprom->pio, eeprom->statemachineId);
    return (result & 0x01) == 0;
}

void eepromReceive(const EEPROM* eeprom, uint8_t* data, bool keepAlive) {
    pio_sm_put_blocking(eeprom->pio, eeprom->statemachineId, keepAlive ? 0x1FF : 0x2FF);
    uint32_t result = pio_sm_get_blocking(eeprom->pio, eeprom->statemachineId);
    *data = result & 0xFF;
}

void eepromReceiveArray(const EEPROM* eeprom, uint8_t* data, uint8_t length) {
    for (size_t i = 0; i < length; i++) {
        eepromReceive(eeprom, data + i, i < (length - 1));
    }
}

int eepromSearchDevice(const EEPROM* eeprom) {
    int deviceAddress = -1;
    for (size_t i = 0; i < 8; i++) {
        sleep_us(150);  // t_htss
        uint32_t payload = (CommandManufacturerIDRead << 4) | (i << 1) | 1;
        bool response = eepromSend(eeprom, payload);
        sleep_us(150);  // t_htss

        if (response) {
            deviceAddress = i;
            break;
        }
    }
    return deviceAddress;
}
