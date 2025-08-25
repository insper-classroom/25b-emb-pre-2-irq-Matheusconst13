#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int led = 4;
const int BTN_PIN_G = 26;
const int led_g = 6;
volatile int flag_r = 0;
volatile int flag_g = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { 
    if (gpio == BTN_PIN_R) {
      flag_r = 1;
    }
  } else if (events == 0x8) {
      if (gpio == BTN_PIN_G) {
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

  gpio_init(led);
  gpio_set_dir(led, GPIO_OUT);  
  int led_status = 0;

  gpio_init(led_g);
  gpio_set_dir(led_g, GPIO_OUT);  
  int led_status_g = 0;

  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

  while (true) {
    if (flag_r) {
      led_status = !led_status;
      gpio_put(led, led_status);  
      flag_r = 0;      
    }
    if (flag_g) {
      printf("verde \n");
      led_status_g = !led_status_g;
      gpio_put(led_g, led_status_g);  
      flag_g = 0;      
    }
  }
}
