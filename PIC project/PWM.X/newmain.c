/*
 * File:   main.c
 * Author: DELL
 *
 * Created on April 25, 2021, 1:13 AM
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
#include "ADC.h"

#define _XTAL_FREQ 8000000
#define TMR2_PRESCALE 4
long PWM_freq = 5000;

void PWM1_Init() {
    PR2 = (_XTAL_FREQ /(PWM_freq * 4 * TMR2_PRESCALE)) - 1; 
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    T2CONbits.T2CKPS0 = 1;
    T2CONbits.T2CKPS1 = 0;
    TMR2ON = 1;
    TRISC2 = 0;
}

void PWM1_dutyCycle(unsigned int duty)
{
    if(duty < 1023)
    {
        duty = ((float)duty /1023)*4*(PR2 + 1);
        CCPR1L = duty >> 2;
        CCP1CONbits.DC1B1 = duty >> 1;
        CCP1CONbits.DC1B0 = duty;
    }
    return;
}

void main(void) {
    TRISA0 = 1;
    ADC_init(FOSC_DIV_8, AN2_AN3);
    PWM1_Init();
    while(1)
    {
        PWM1_dutyCycle(ADC_read(0));
        __delay_ms(5);
    }
    return;
}
