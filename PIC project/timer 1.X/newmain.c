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

#include <xc.h>
#include <stdio.h>
#include "lcd.h"

char buffer[16];

void main(void) {
    unsigned int cnt;
    TRISCbits.TRISC0 = 1;
    TRISD = 0;
    lcd_init();
    T1CONbits.TMR1CS = 1;
    T1CONbits.T1CKPS = 0;
    T1CONbits.T1SYNC = 0;
    T1CONbits.TMR1ON = 1;
    TMR1H = TMR1L = 0;
    while(1)
    {
        cnt = TMR1;
//        cnt = TMR1H;
//        cnt << 8;
//        cnt |= TMR1L;
        sprintf(buffer,"Dem xung: %d",cnt);
        lcd_com(FIRST_LINE);
        lcd_puts(buffer);
    }
}
