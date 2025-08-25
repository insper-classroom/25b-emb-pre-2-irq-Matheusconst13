#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int led = 4;
volatile int fall = 0;


void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { 
    fall = 1;
  }
} 

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(led);
  gpio_set_dir(led, GPIO_OUT);  
  int led_status = 0;


  while (true) {
    gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    if (fall){

      led_status = !led_status;
      gpio_put(led, led_status);  
      fall = 0;
      
    }

  }
}
