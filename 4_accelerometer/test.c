#include "test.h"

#include "uart.h"
#include "gpio.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

bool lights_on = false;

void test_init() {
	// Configure LED Matrix
	for(int i = 4; i <= 15; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}
}

void test_lights_on() {
	for(int i = 13; i <= 15; i++){
		GPIO->OUTSET = (1 << i);
	}	

	lights_on = true;
}

void test_lights_off() {
	for(int i = 13; i <= 15; i++){
		GPIO->OUTCLR = (1 << i);
	}	

	lights_on = false;
}


void test_toggle_lights() {
	if (!lights_on) {
		for(int i = 13; i <= 15; i++){
			GPIO->OUTSET = (1 << i);
		}		
	}
	else {
		for(int i = 13; i <= 15; i++){
			GPIO->OUTCLR = (1 << i);
		}	
	}

	lights_on = !lights_on;
}


