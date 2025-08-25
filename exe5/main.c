#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"

volatile int flag_saida = 0;
const int BTN_PIN = 28;

volatile  uint64_t tempo_entrada = 0;
volatile int tempo_saida = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_PIN) {
        if (events == GPIO_IRQ_EDGE_FALL) { 
            tempo_entrada = time_us_64();  
        }if (events == GPIO_IRQ_EDGE_RISE) {
            flag_saida = 1;
        }
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    while (true) {
        if (flag_saida) {
            uint64_t duracao = (time_us_64()- tempo_entrada)/1000;
            if (duracao >= 800) { 
                printf("Aperto longo!\n");
            } else {
                printf("Aperto curto!\n");
            }
            flag_saida = 0;
        }
    }
}
