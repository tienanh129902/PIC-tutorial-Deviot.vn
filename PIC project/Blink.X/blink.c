/*
 * File:   main.c
 * Author: DELL
 *
 * Created on April 25, 2021, 1:59 AM
 */

// CONFIG
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

#define _XTAL_FREQ 20000000
#define TMR2_PRESCALE 4
#include <xc.h>

typedef enum {
    SINGLE_OUTPUT,
    FULL_BRIDGE_FORWARD,
    HALF_BRIDGE,
    FULL_BRIDGE_REVERSE,
} PWM_outputMode;

typedef enum {
    ACH_BDH,
    ACH_BDL,
    ACL_BDH,
    ACL_BDL
} PWM_pinActiveMode;
void PWM_H_init(PWM_outputMode o, PWM_pinActiveMode p);
void PWM_H_duty(unsigned int duty);
long PWM_freq = 5000;

void adc_init() {
    ADCON0 = 0x41; 
    ADCON1 = 0xF0; 
}

int adc_readChannel(int channel) {
    if (channel > 13) return 0;
    ADCON0 &= 0xC1; 
    ADCON0 |= channel << 2; 
    __delay_ms(2);
    GO_nDONE = 1; 
    while (GO_nDONE); 
    return ((ADRESH << 8) + ADRESL); 
}

void PWM_H_init(PWM_outputMode o, PWM_pinActiveMode p) {
    CCP1CONbits.P1M = o;
    CCP1CONbits.CCP1M0 = p;
    CCP1CONbits.CCP1M1 = p >> 1;
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    PR2 = (_XTAL_FREQ / (PWM_freq * TMR2_PRESCALE * 4)) - 1;
    T2CONbits.T2CKPS = 1;
    T2CONbits.TMR2ON = 1;
    TRISC2 = 0;
    TRISD5 = 0;
    TRISD6 = 0;
    TRISD7 = 0;
}

void PWM_H_duty(unsigned int duty) {
    if (duty < 1023) {
        duty = ((float) duty / 1023)*(_XTAL_FREQ / (PWM_freq * TMR2_PRESCALE));
        CCP1CONbits.DC1B1 = (duty >> 1)&1;
        CCP1CONbits.DC1B0 = duty & 1;
        CCPR1L = duty >> 2;
    }
    return;
}

void main(void) {    
    TRISA0 = 1;
    adc_init();
    PWM_H_init(FULL_BRIDGE_FORWARD, ACH_BDL);
    while (1) {        
        PWM_H_duty(adc_readChannel(0));
        __delay_ms(50);
    }
    return;
}
