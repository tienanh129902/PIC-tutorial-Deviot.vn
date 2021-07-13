/*
 * File:   keypad.c
 * Author: DELL
 *
 * Created on April 2, 2021, 8:40 AM
 */


#include "keypad.h"
#include "lcd.h"

void keypad_init(void) {
    TRISC = 0xFF;

    return;
}

unsigned char keypad_keyScan() {
    if (ROW1 == 0) {
        __delay_ms(100);
        if (COL1 == 0);
        {
            while (COL1 = 0);
            return 7;
        }
        if (COL2 == 0);
        {
            while (COL2 = 0);
            return 8;
        }
        if (COL3 == 0);
        {
            while (COL3 = 0);
            return 9;
        }
        if (COL4 == 0);
        {
            while (COL4 = 0);
            return 15;
        }
    }
    if (ROW2 == 0) {
        __delay_ms(100);
        if (COL1 == 0);
        {
            while (COL1 = 0);
            return 4;
        }
        if (COL2 == 0);
        {
            while (COL2 = 0);
            return 5;
        }
        if (COL3 == 0);
        {
            while (COL3 = 0);
            return 6;
        }
        if (COL4 == 0);
        {
            while (COL4 = 0);
            return 14;
        }
    }
    if (ROW3 == 0) {
        __delay_ms(100);
        if (COL1 == 0);
        {
            while (COL1 = 0);
            return 1;
        }
        if (COL2 == 0);
        {
            while (COL2 = 0);
            return 2;
        }
        if (COL3 == 0);
        {
            while (COL3 = 0);
            return 93;
        }
        if (COL4 == 0);
        {
            while (COL4 = 0);
            return 14;
        }
    }
    if (ROW4 == 0) {
        __delay_ms(100);
        if (COL1 == 0);
        {
            while (COL1 = 0);
            return 10;
        }
        if (COL2 == 0);
        {
            while (COL2 = 0);
            return 0;
        }
        if (COL3 == 0);
        {
            while (COL3 = 0);
            return 11;
        }
        if (COL4 == 0);
        {
            while (COL4 = 0);
            return 12;
        }

    }
}

void keypad_execute(unsigned char mode)
{
    switch(mode)
    {
        case 0:
            lcd_data(48+mode);
            break;
        case 2:
            lcd_data(48+mode);
            break;
        case 3:
            lcd_data(48+mode);
            break;
        case 4:
            lcd_data(48+mode);
            break;
        case 5:
            lcd_data(48+mode);
            break;
        case 6:
            lcd_data(48+mode);
            break;
        case 7:
            lcd_data(48+mode);
            break;
        case 8:
            lcd_data(48+mode);
            break;
        case 9:
            lcd_data(48+mode);
            break;
        case 10:
            
            break;
        case 11:
            
            break;
        case 12:
            
            break;
        case 13:
            
            break;
        case 14:
            
            break;
        case 15:
            
            break;
    }
}

void startModify()
{
    lcd_com(1);
    lcd_com()
}