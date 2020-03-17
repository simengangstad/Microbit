#include "gpio.h"
#include "gpiote.h"
#include "ppi.h"

#define BUTTON_A 17
#define BUTTON_B 26

#define LED_MATRIX_ROW_1 13
#define LED_MATRIX_ROW_2 14
#define LED_MATRIX_ROW_3 15

#define HI_TO_LO 2
#define TOGGLE 3

int main() {
    // Set ground pins to output and logical low
	for(int i = 4; i <= 12; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}	

	// Configure event on channel 0
	GPIOTE->INTENSET = 1;
	// Enable as event
	GPIOTE->CONFIG[0] = 1;
	// Set input to button A
	GPIOTE->CONFIG[0] = GPIOTE->CONFIG[0] | BUTTON_A << 8;
	// Set polarity to HiToLo
	GPIOTE->CONFIG[0] = GPIOTE->CONFIG[0] | HI_TO_LO << 16;


	// Configure task on channel 1, 2 and 3
	GPIOTE->INTENSET = 0b111 << 1;
	// Enable as task 
	GPIOTE->CONFIG[1] = 3;
	GPIOTE->CONFIG[2] = 3;
	GPIOTE->CONFIG[3] = 3;
	// Set output to rows in the LED matrix 
	GPIOTE->CONFIG[1] = GPIOTE->CONFIG[1] | LED_MATRIX_ROW_1 << 8;
	GPIOTE->CONFIG[2] = GPIOTE->CONFIG[2] | LED_MATRIX_ROW_2 << 8;
	GPIOTE->CONFIG[3] = GPIOTE->CONFIG[3] | LED_MATRIX_ROW_3 << 8;
	// Set polarity to toggle 
	GPIOTE->CONFIG[1] = GPIOTE->CONFIG[1] | TOGGLE << 16;
	GPIOTE->CONFIG[2] = GPIOTE->CONFIG[2] | TOGGLE << 16;
	GPIOTE->CONFIG[3] = GPIOTE->CONFIG[3] | TOGGLE << 16;


	// Link them through PPI
	// Enable channels 1, 2 and 3
	PPI->CHENSET = 0b1110;

	// Link event enpoint and task endpoint for the respective LED rows.
	PPI->PPI_CH[1].EEP = (uint32_t)&(GPIOTE->IN[0]);
	PPI->PPI_CH[1].TEP = (uint32_t)&(GPIOTE->OUT[1]);

	PPI->PPI_CH[2].EEP = (uint32_t)&(GPIOTE->IN[0]);
	PPI->PPI_CH[2].TEP = (uint32_t)&(GPIOTE->OUT[2]);

	PPI->PPI_CH[3].EEP = (uint32_t)&(GPIOTE->IN[0]);
	PPI->PPI_CH[3].TEP = (uint32_t)&(GPIOTE->OUT[3]);

	// Configure buttons
	GPIO->PIN_CNF[17] = 0;
	GPIO->PIN_CNF[26] = 0;

	int sleep = 0;

	while(1) {
		sleep = 10000;
		while(--sleep);
	}
	return 0;
}
