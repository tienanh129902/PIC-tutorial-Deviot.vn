/*
 * File:   main.c
 * Author: DELL
 *
 * Created on May 16, 2021, 5:32 PM
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
#include "lcd.h"
#define _XTAL_FREQ 8000000

char temp = 0;
char cnt = 0;
char buffer1[16];
char buffer2[16];

//void __interrupt() isr() {
//    if (INTCONbits.INTF == 1) {
//        cnt++;
//        INTCONbits.INTF = 0;
//    }
//}

void main(void) {
    TRISD = 0;
    TRISB = 0xFF;
    ANSEL =ANSELH =0;
    INTCONbits.GIE = 1;
    INTCONbits.INTE = 1;
    INTCONbits.INTF = 0;
    OPTION_REGbits.INTEDG = 0;
    lcd_init();
    while (1) {
        if (RB0 == 0) {
            eeprom_write(0, cnt); // ghi gia tri doc duoc cua adc vao dia chi 0 cua EEPROM
        }
        if (RB4 == 0) {
            eeprom_write(0, 0); // ghi gia tri 0 vao dia chi 0 cua EEPROM
        }
        temp = (char)eeprom_read(0);
        sprintf(buffer1, "Now: %d", cnt);
        lcd_com(FIRST_LINE);
        lcd_puts(buffer1);
        __delay_ms(5);
        sprintf(buffer2, "Then: %d", temp);
        lcd_com(SECOND_LINE);
        lcd_puts(buffer2);
        cnt++;
        __delay_ms(1000);
    }
    return;
}
