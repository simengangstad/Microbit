#include "uart.h"
#include "gpio.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define BUTTON_A 17
#define BUTTON_B 26

bool lights_on = false;

void toggle_lights() {
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

ssize_t _write(int fd, const void *buf, size_t count) {
    char* letter = (char *) (buf);

    for (int i = 0; i < count; i++) {
        uart_send(*letter);
        letter++;
    }

    return count;
}

int main() {
    // Configure LED Matrix
	for(int i = 4; i <= 15; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}

	// Configure buttons
	GPIO->PIN_CNF[17] = 0;
	GPIO->PIN_CNF[26] = 0;

    uart_init();

	int sleep = 0;

	while(1) {
		iprintf("The chemical formula for Ketamine is C%dH%dClNO\n\r", 13, 16);


		if (~(GPIO->IN) & (1<<BUTTON_A)) {
			uart_send('A');
		}
		else if (~(GPIO->IN) & (1<<BUTTON_B)) {
			uart_send('B');
		}
		

        if (uart_read() != '\0') {
            toggle_lights();
        }

		sleep = 10000;
		while(--sleep);
	}
	return 0;
}