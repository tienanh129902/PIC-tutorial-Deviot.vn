/*
 * File:   main.c
 * Author: DELL
 *
 * Created on April 26, 2021, 9:42 PM
 */

// CONFIG1
#pragma config FOSC = LP        // Oscillator Selection bits (LP oscillator: Low-power crystal on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = ON          // Code Protection bit (Program memory code protection is enabled)
#pragma config CPD = ON         // Data Code Protection bit (Data memory code protection is enabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT = HALF       // Flash Program Memory Self Write Enable bits (0000h to 0FFFh write protected, 1000h to 1FFFh may be modified by EECON control)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#include "ADC.h"
#define TMR2_PRESCALE 4
long PWM_freq = 5000;

typedef enum {
    SINGLE_OUTPUT,
    FULL_BRIDGE_FORWARD,
    HALF_BRIDGE,
    FULL_BRIDGE_REVERSE
} PWM_outputMode;

typedef enum {
    ACH_BDH,
    ACH_BDL,
    ACL_BDH,
    ACL_BDL
} PWM_pinActiveMode;

void PWM1_H_init(PWM_outputMode m, PWM_pinActiveMode p);
void PWM1_H_duty(unsigned int duty);

void PWM1_H_init(PWM_outputMode m, PWM_pinActiveMode p) {
    CCP1CONbits.P1M = m;
    CCP1CONbits.CCP1M1 = p >> 1;
    CCP1CONbits.CCP1M0 = p;
    CCP1CONbits.CCP1M2 = CCP1CONbits.CCP1M3 = 1;
    PR2 = (_XTAL_FREQ / (PWM_freq * TMR2_PRESCALE * 4)) - 1;
    T2CONbits.T2CKPS = 3;
    T2CONbits.TMR2ON = 1;
    TRISC2 = 0;
    TRISD5 = TRISD6 = TRISD7 = 0;
}

void PWM1_H_duty(unsigned int duty)
{
    if(duty < 1023)
    {
        duty = ((float)duty /1023)*(_XTAL_FREQ/(PWM_freq * TMR2_PRESCALE));
        CCP1CONbits.DC1B1 = duty >> 1;
        CCP1CONbits.DC1B0 = duty;
        CCPR1L = duty >> 2;
    }
    return;
}

void main(void) {
    TRISA0 = 1;
    ADC_init(FOSC_DIV_8,AN2_AN3);
    PWM1_H_init(FULL_BRIDGE_FORWARD, ACL_BDL);
    while(1)
    {
        PWM1_H_duty(ADC_read(0));
        __delay_ms(50);
    }
    return;
}
