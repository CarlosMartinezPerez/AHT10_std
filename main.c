#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/aht10.h"

int main() {
    stdio_init_all();
    sleep_ms(5000);

    aht10_i2c_init();

    ssd1306_init();
    ssd1306_clear();
    ssd1306_draw_string(0, 0, "=== AHT10 === ");
    ssd1306_show();
    
    printf("======A HT10 TESTE ======\n");

    float temp;
    float umid;
    float orv;
    char linha1[16];
    char linha2[16];
    char linha3[16];

    while (1) {
        aht10_read();

        temp = aht10_get_temperature();
        umid = aht10_get_humidity();
        orv  = aht10_get_dewpoint();
        
        printf("Umidade = %.2f %%\n", umid);
        snprintf(linha1, sizeof(linha1), "Umid = %.2f %%", umid);
        printf("Temperatura = %.2f C\n", temp);
        snprintf(linha2, sizeof(linha2), "Temp = %.2f C", temp);
        printf("Ponto de Orvalho = %.2f C\n", orv);
        snprintf(linha3, sizeof(linha3), "Pt Orv = %.2f C", orv);

        ssd1306_draw_string(0,  16, linha1);
        ssd1306_draw_string(0,  24, linha2);
        ssd1306_draw_string(0,  32, linha3);
        ssd1306_show();
        sleep_ms(1000);
    }

}