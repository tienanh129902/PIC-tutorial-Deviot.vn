/*
 * File:   main.c
 * Author: DELL
 *
 * Created on May 5, 2021, 2:22 PM
 */

// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#include <xc.h>
#define _XTAL_FREQ 8000000

void main(void) {
    TRISCbits.TRISC2 = 0;
    CCP1CON = 0x09;
    PIR1bits.CCP1IF = 0;
    TMR1 = 0;
    T1CON = 0xC0;
    CCPR1 = 800; // 0,4ms
    T1CONbits.TMR1ON = 1;
    while (1) {
        while (!PIR1bits.CCP1IF);
        PIR1bits.CCP1IF = 0;
        TMR1 = 0;
        CCP1CON = 0x08;
        CCPR1 = 1200; // 0,6ms
        
        while (!PIR1bits.CCP1IF);
        PIR1bits.CCP1IF = 0;
        TMR1 = 0;
        CCP1CON = 0x09;
        CCPR1 = 800; // 0,4ms
    }
    return;
}