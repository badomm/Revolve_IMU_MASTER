/*
 * output_led.h
 *
 * Created: 3/23/2015 13:25:21
 *  Author: Jimmy Chau
 */ 


#ifndef OUTPUT_LED_H_
#define OUTPUT_LED_H_
#include "sam4e_base/RevolveDrivers/pio.h"

#define LED0(state) pio_setOutput(PIOC, 4, state)
#define LED1(state) pio_setOutput(PIOC, 5, state)
#define LED2(state) pio_setOutput(PIOC, 6, state)
#define LED3(state) pio_setOutput(PIOC, 7, state)

#define toggle_LED0() led_toggle(PIOC, 4)
#define toggle_LED1() led_toggle(PIOC, 5)
#define toggle_LED2() led_toggle(PIOC, 6)
#define toggle_LED3() led_toggle(PIOC, 7)

void led_init();
void led_toggle(Pio * pio,int pin);
void led_write(uint8_t i);

#endif /* OUTPUT_LED_H_ */