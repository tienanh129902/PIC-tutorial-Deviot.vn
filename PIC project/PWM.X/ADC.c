/*
 * File:   ADC.c
 * Author: DELL
 *
 * Created on April 19, 2021, 3:02 PM
 */


#include "ADC.h"

void ADC_init(ADC_source s,ADC_ref r)
{
    ADCON0bits.ADON = 1;
    ADCON0bits.ADCS = s;
    ADCON1bits.VCFG1 = r >> 1;
    ADCON1bits.VCFG0 = r;
    ADCON1bits.ADFM = 1;
}
int ADC_read(char channel)
{
    if(channel > 13 || channel <0)
    {
        return 0;
    }
    else
    {
        ADCON0bits.CHS = channel;
        __delay_ms(2);
        ADCON0bits.GO_nDONE = 1;
        while (GO_nDONE);
        return ((ADRESH << 8)+ ADRESL);
    }
}