#ifndef AHT10_H
#define AHT10_H

#include <math.h>
#include "hardware/i2c.h"

#define ADDR 0x38
#define WATER_VAPOR 17.62f
#define BAROMETRIC_PRESSURE 243.5f

#define I2C_PORT i2c0
#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1

void aht10_i2c_init(void);
void aht10_read(void);          // lê brutos uma vez e atualiza buffers locais
float aht10_get_temperature(void);
float aht10_get_humidity(void);
float aht10_get_dewpoint(void);

#endif
