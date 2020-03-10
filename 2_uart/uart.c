#include "uart.h"
#include <stdint.h>
#include <stdio.h>
#include "gpio.h"

#define UART ((NRF_UART_REG*) 0x40002000)
#define TGT_TXD_PIN 24
#define TGT_RXD_PIN 25


typedef struct {
    volatile uint32_t STARTRX;
    volatile uint32_t STOPRX;
    volatile uint32_t STARTTX;
    volatile uint32_t STOPTX;
    volatile uint32_t UNUSED0[3];
    volatile uint32_t SUSPEND;

    volatile uint32_t UNUSED1[56];

    volatile uint32_t CTS;
    volatile uint32_t NCTS;
    volatile uint32_t RXDRDY;
    volatile uint32_t UNUSED2[4];
    volatile uint32_t TXDRDY;
    volatile uint32_t UNUSED3[1];
    volatile uint32_t ERROR;
    volatile uint32_t UNUSED4[7];
    volatile uint32_t RXTO;

    volatile uint32_t UNUSED5[110];

    volatile uint32_t INTEN;
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t UNUSED6[93];
    volatile uint32_t ERRORSRC;
    volatile uint32_t UNUSED7[31];
    volatile uint32_t ENABLE;
    volatile uint32_t UNUSED8[1];
    volatile uint32_t PSELRTS;
    volatile uint32_t PSELTXD;
    volatile uint32_t PSELCTS;
    volatile uint32_t PSELRXD;
    volatile uint32_t RXD;
    volatile uint32_t TXD;
    volatile uint32_t UNUSED9[1];
    volatile uint32_t BAUDRATE;
    volatile uint32_t UNUSED10[17];
    volatile uint32_t CONFIG;
} NRF_UART_REG;

void uart_init() {
    UART->BAUDRATE = 0x00275000;
    
    GPIO->DIRSET = (1 << TGT_TXD_PIN);
    GPIO->DIRSET = (0 << TGT_RXD_PIN);

    UART->PSELTXD = TGT_TXD_PIN;
    UART->PSELRXD = TGT_RXD_PIN;

    UART->PSELRTS = 0xFFFFFFFF;
    UART->PSELCTS = 0xFFFFFFFF;

    UART->ENABLE = 4;

    UART->STARTRX = 1;
}

void uart_send(char letter) {
    UART->STARTTX = 1;

    UART->TXD = letter;
    UART->TXDRDY = 0;

    while(!(UART->TXDRDY)) {};

    UART->STOPTX = 1;
}

void uart_send_string(char* string) {
    for (int i = 0; string[i] != '\0'; i++) {
        uart_send(string[i]);
    }
    return;
}

char uart_read() {
    if(!(UART->RXDRDY)) {
        return '\0';
    }

    UART->RXDRDY = 0;

    return (char) UART->RXD;
}