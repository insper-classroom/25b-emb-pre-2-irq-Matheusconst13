#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
volatile int flag_r = 0;
volatile int flag_g = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {  // fall edge
        if (gpio == BTN_PIN_R) {
            flag_r = 1;
        } else if (gpio == BTN_PIN_G) {
            flag_g = 1;
        }
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    // callback led r (first)
    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);

    // callback led g (nao usar _with_callback)
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    while (true) {
        if (flag_r){
            printf("fall red\n");
            flag_r = 0;
        }
        if (flag_g){
            printf("fall green\n");
            flag_g = 0;
        }
        
    }
}