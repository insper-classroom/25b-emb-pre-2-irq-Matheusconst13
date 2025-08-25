#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"

volatile int flag_entrada = 0;
volatile int flag_saida = 0;
const int BTN_PIN = 28;

uint32_t tempo_entrada = 0;
uint32_t tempo_saida = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_PIN) {
        if (events == GPIO_IRQ_EDGE_FALL) { 
            tempo_entrada = time_us_32();  
            flag_entrada = 1;
        } else if (events == GPIO_IRQ_EDGE_RISE) {
            tempo_saida = time_us_32();  
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
        if (flag_entrada) {
            flag_entrada = 0;
            while (!flag_saida) {} 

            flag_saida = 0;

            uint32_t duracao = tempo_saida - tempo_entrada;

            if (duracao > 800000) { 
                printf("Aperto longo!\n");
            } else {
                printf("Aperto curto!\n");
            }
        }
    }
}
