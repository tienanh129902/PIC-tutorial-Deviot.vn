/*
 * File:   LCD.c
 * Author: DELL
 *
 * Created on May 30, 2021, 12:21 AM
 */


#include "LCD.h"
#define LCDPORT PORTD
#define RS RD0
#define RW RD1
#define E RD2
#define lcd_delay 300
#define blankSpace ' '

__bit status = 0;

void lcd_init() {
    delay(lcd_delay);
    lcd_init_write(0x30);
    delay(lcd_delay);
    lcd_init_write(0x30);
    delay(lcd_delay);
    lcd_init_write(0x30);
    delay(lcd_delay);
    lcd_init_write(0x20);
    delay(lcd_delay);
    lcd_com(LCD_TURN_ON);
    delay(lcd_delay);
    lcd_com(0x28);
    delay(lcd_delay);
    lcd_com(LCD_ENTRY_MODE_SET);
    delay(lcd_delay);
    lcd_com(0x85);
    delay(lcd_delay);
    lcd_com(6);
    delay(lcd_delay);
    lcd_com(LCD_CLEAR);
    delay(lcd_delay);
}

void lcd_init_write(unsigned char a) {
    RS = 0;
    RW = 0;
    LCDPORT = a;
    E = 1;
    delay(lcd_delay);
    E = 0;
}

void delay(unsigned int j) {
    unsigned int i = 0;
    for (i = 0; i < j; i++);
}

void lcd_com(unsigned char a) {
    unsigned char temp;
    if (status) {
        status = 0;
        goto next;
    }
    RS = 0;
next:
    RW = 0;
    temp = a;
    temp &= 0xf0;
    LCDPORT &= 0x0f;
    LCDPORT |= temp;
    E = 1;
    delay(lcd_delay);
    E = 0;
    temp = a << 4;
    temp &= 0xf0;
    LCDPORT &= 0x0f;
    LCDPORT |= temp;
    E = 1;
    delay(lcd_delay);
    E = 0;
}

void lcd_data(unsigned char a) {
    status = 1;
    RS = 1;
    lcd_com(a);
}

void lcd_puts(char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}

void lcd_set_cursor(char row, char col) {
    switch (row) {
        case 4:
            lcd_com(LCD_FOURTH_ROW + col - 1);
            break;
        case 2:
            lcd_com(SECOND_LINE + col - 1);
            break;
        case 3:
            lcd_com(LCD_THIRD_ROW + col - 1);
            break;
        default:
            lcd_com(FIRST_LINE + col - 1);
    }
}
