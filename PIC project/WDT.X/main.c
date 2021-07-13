/*
 * File:   main.c
 * Author: DELL
 *
 * Created on May 13, 2021, 5:51 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
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
#include "lcd.h"
#define _XTAL_FREQ 8000000

void main(void) {
    int count = 0;
    TRISD = 0;
    TRISC6 = 0;
    lcd_init();
    WDTCONbits.SWDTEN = 1;
    if(STATUSbits.nTO){
        lcd_com(FIRST_LINE);
        lcd_puts("WDT reset");
    }
    __delay_ms(1000);
    OPTION_REGbits.PSA = 1;
    OPTION_REGbits.PS = 0b111; //128
    WDTCONbits.WDTPS = 0b0100; // 512
    RC6 = 0;
    lcd_com(LCD_CLEAR);
    while(1)
    {
        RC6 = !RC6;
        __delay_ms(200);
        CLRWDT();
        count++;
        if(count > 10) while(1);
    }
    return;
}
