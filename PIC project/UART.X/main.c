/*
 * File:   main.c
 * Author: DELL
 *
 * Created on May 5, 2021, 3:43 PM
 */

// CONFIG1
#pragma config FOSC = EXTRC_CLKOUT// Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 8000000

void UART_init(long baud)
{
    TRISC6 = 0;
    TRISC7 = 1;
    SYNC = 0;
    BRGH = 0;
    BRG16 = 0;
    SPBRG = (_XTAL_FREQ / (64*baud)) -1;
    SPEN = 1;
    TXEN = 1;
    CREN = 1;
    TX9 = RX9 = 0;
}

void UART_sendChar(char data)
{
    while(!TXIF);
    TXREG = data;
}

void UART_sendString(char *str)
{
    while(*str)
    {
        UART_sendChar(*str++);
    }
}

char UART_getChar()
{
    if(OERR)
    {
        CREN = 0;
        CREN = 1;
    }
    while(!RCIF);
    return RCREG;
}

void main(void) {
    UART_init(9600);
    TRISD4 = 0;
    while(1)
    {
        if(UART_getChar()=='1')
        {
            RD4 = 1;
            UART_sendString("LED on ");
            
        }
        if(UART_getChar()=='0')
        {
            RD4 = 0;
            UART_sendString("LED off ");
            
        }
        __delay_ms(1000);
    }
    return;
}
