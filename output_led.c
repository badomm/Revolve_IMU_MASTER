/*
 * output_led.c
 *
 * Created: 3/23/2015 13:02:14
 *  Author: Jimmy Chau
 */ 
#include "output_led.h"

void led_init(){
	pio_init();
	pio_enableOutput(PIOC, 4);
	pio_enableOutput(PIOC, 5);
	pio_enableOutput(PIOC, 6);
	pio_enableOutput(PIOC, 7);
}

void led_toggle(Pio * pio, int pin){
	if (pio_readPin(pio, pin)){
		pio_setOutput(pio, pin, LOW);
	}else{
		pio_setOutput(pio, pin, HIGH);
	}
}

void led_write(uint8_t i){
	LED0(i & 0b1000);
	LED1(i & 0b0100);
	LED2(i & 0b0010);
	LED3(i & 0b0001);
}