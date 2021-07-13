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
#define _XTAL_FREQ 8000000

//void delayTimer0_ms(unsigned int t)
//{
//    while(t--)
//    {
//        TMR0 = 5;
//        INTCONbits.T0IF = 0;
//        while(!INTCONbits.T0IF);
//    }
//}
unsigned int cnt = 0;
void __interrupt() isr()
{
    if(INTCONbits.T0IF == 1)
    {
        TMR0 = 5;
        INTCONbits.T0IF = 0;
        cnt++;
    }
}
void main(void) {
    TRISD = 0;
    PORTD = 0;
    OPTION_REGbits.PS = 0b010;
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    INTCONbits.GIE = 1;
    INTCONbits.T0IE =1;
    INTCONbits.T0IF = 0;
    while(1)
    {
        if(cnt == 1000)
        {
            PORTD =~PORTD;
            cnt = 0;
        }
    }
    return;
}
