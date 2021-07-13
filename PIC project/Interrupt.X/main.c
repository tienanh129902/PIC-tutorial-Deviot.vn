/*
 * File:   main.c
 * Author: DELL
 *
 * Created on April 1, 2021, 9:25 PM
 */


// CONFIG1
#pragma config FOSC = EXTRC_CLKOUT// Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
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

static unsigned char cnt = 0;

void __interrupt() isr()
{
    if(INTCONbits.INTF == 1)
    {
//        PORTDbits.RD0 = ! PORTDbits.RD0;
        cnt ++;
        INTCONbits.INTF = 0;
    }
    if(cnt == 3) cnt = 0;
}
void xenke()
{
    PORTD = 0x55;
    __delay_ms(500);
    PORTD = 0xAA;
    __delay_ms(500);
}
void main(void) {
    TRISBbits.TRISB0 = 1;
    TRISD = 0;
//    PORTDbits.RD0 = 0;
    INTCONbits.GIE = 1;
    INTCONbits.INTE = 1;
    INTCONbits.INTF = 0;
    OPTION_REGbits.INTEDG = 0;
    ANSELHbits.ANS12 = 0;
    while(1)
    {
        switch(cnt)
        {
            case 1:
                PORTD = 0xFF;
                break;
            case 2:
                xenke();
                break;
            default:
                PORTD = 0;
                break;
        }
    }
    return;
}
