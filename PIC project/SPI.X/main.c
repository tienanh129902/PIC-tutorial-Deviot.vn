/*
 * File:   main.c
 * Author: DELL
 *
 * Created on May 3, 2021, 3:31 PM
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
#include <stdio.h>
#include <string.h>
#include "SPI.h"
#include "lcd.h"

#define _XTAL_FREQ 8000000
#define SS1 RC6
#define SS2 RC7

char buffer[16];

int readTemp() {
    int data;
    char high, low;
    SS2 = 0;
    __delay_ms(1);
    high = SPI_transfer(0);
    low = SPI_transfer(0);
    SS2 = 1;
    data = (high << 8) | low;
    return (data >> 5) - 25;
}

void main(void) {
    TRISD = 0;
    TRISC6 = TRISC7 = 0;
    lcd_init();
    SPI_init(SPI_MASTER_OSC_DIV4, DATA_SAMPLE_END, CLOCK_IDLE_LOW, SPI_IDLE_TO_ACTIVE);
    lcd_com(FIRST_LINE);
    lcd_puts("Deviot.vn");
    while(1)
    {
        SS1 = 1;
        int temp = readTemp();
        sprintf(buffer, "Temperature: %d ", temp);
        lcd_com(SECOND_LINE);
        lcd_puts(buffer);
    }
    return;
}
