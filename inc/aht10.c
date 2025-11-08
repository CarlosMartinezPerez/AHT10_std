#include "pico/stdlib.h"
#include "aht10.h"

// comando medida
static uint8_t cmd_measure[3] = {0xAC, 0x33, 0x00};

// buffers locais últimos valores
static float last_temp = NAN;
static float last_hum  = NAN;

void aht10_i2c_init(void){
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    sleep_ms(50);
}

void aht10_read(void) {
    uint8_t raw[6];

    // trigger conversion
    i2c_write_blocking(I2C_PORT, ADDR, cmd_measure, 3, false);
    sleep_ms(100); // tempo típico para conversão

    // read result
    i2c_read_blocking(I2C_PORT, ADDR, raw, 6, false);

    // parse humidity
    uint32_t hum_raw = ((uint32_t)raw[1] << 16) | ((uint32_t)raw[2] << 8) | raw[3];
    hum_raw >>= 4;
    last_hum = (hum_raw * 100.0f) / 1048576.0f;

    // parse temp
    uint32_t temp_raw = ((uint32_t)(raw[3] & 0x0F) << 16) | ((uint32_t)raw[4] << 8) | raw[5];
    last_temp = ((200.0f * temp_raw) / 1048576.0f) - 50.0f;
}

float aht10_get_temperature(void) {
    return last_temp;
}

float aht10_get_humidity(void) {
    return last_hum;
}

float aht10_get_dewpoint(void) {
    float h = last_hum;
    float t = last_temp;
    float gamma = log(h/100.0f) + WATER_VAPOR * t / (BAROMETRIC_PRESSURE + t);
    return BAROMETRIC_PRESSURE * gamma / (WATER_VAPOR - gamma);
}