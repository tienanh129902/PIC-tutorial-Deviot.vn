/*
 * File:   I2C_LCD.c
 * Author: DELL
 *
 * Created on May 12, 2021, 9:30 PM
 */

#include "I2C_LCD.h"

char I2C_add, RS;

void I2C_Master_Init(long int baud) {
    SSPSTAT = 0;
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = ((_XTAL_FREQ / 4) / baud) - 1;
    TRISC3 = TRISC4 = 1;
}

void I2C_Master_Wait() {
    while ((SSPSTAT & 0b00000100) || (SSPCON2 & 0b00011111));
}

void I2C_Master_Start() {
    I2C_Master_Wait();
    SEN = 1;
}

void I2C_Master_Stop() {
    I2C_Master_Wait();
    PEN = 1;
}

void I2C_ACK() {
    ACKDT = 0;
    I2C_Master_Wait();
    ACKEN = 1;
}

void I2C_NACK() {
    ACKDT = 1;
    I2C_Master_Wait();
    ACKEN = 1;
}

unsigned char I2C_Read_Byte() {
    I2C_Master_Wait();
    RCEN = 1;
    while (!SSPIF);
    SSPIF = 0;
    I2C_Master_Wait();
    return SSPBUF;
}

unsigned char I2C_Master_Write(unsigned char data) {
    I2C_Master_Wait();
    SSPBUF = data;
    while (!SSPIF);
    SSPIF = 0;
    return ACKSTAT;
}

void I2C_Slave_Init(int address) {
    SSPSTAT = 0x80;
    SSPADD = address;
    SSPCON = 0x36;
    SSPCON2 = 0x01;
    TRISC3 = TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIE = 1;
    SSPIF = 0;
}

void LCD_Init(char address) {
    I2C_add = address;
    IO_expanderWrite(0);
    __delay_ms(30);
    LCD_com(0x30);
    __delay_ms(5);
    LCD_com(0x30);
    __delay_ms(5);
    LCD_com(0x30);
    __delay_ms(5);
    LCD_com(LCD_RETURN_HOME);
    __delay_ms(5);
    LCD_com(0x20 | LCD_TYPE << 2);
    __delay_ms(50);
    LCD_com(LCD_TURN_ON);
    __delay_ms(50);
    LCD_com(LCD_CLEAR);
    __delay_ms(50);
    LCD_com(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
    __delay_ms(50);
}

void LCD_writeNibble(char nibble) {
    nibble |= RS;
    IO_expanderWrite(nibble | 0x04);
    IO_expanderWrite(nibble & 0xFB);
    __delay_us(50);
}

void LCD_com(char com) {
    RS = 0;
    LCD_writeNibble(com & 0xf0);
    LCD_writeNibble((com << 4) & 0xf0);
}

void LCD_setCursor(char row, char col) {
    switch (row) {
        case 2:
            LCD_com(LCD_SECOND_ROW + col - 1);
            break;
        case 3:
            LCD_com(LCD_THIRD_ROW + col - 1);
            break;
        case 4:
            LCD_com(LCD_FOURTH_ROW + col - 1);
            break;
        default:
            LCD_com(LCD_FIRST_ROW + col - 1);
    }
}

void LCD_putChar(char data) {
    RS = 1;
    LCD_writeNibble(data & 0xF0);
    LCD_writeNibble((data << 4) & 0xF0);
}

void LCD_puts(char *str) {
    while (*str) {
        LCD_putChar(*str++);
    }
}

void IO_expanderWrite(char data) {
    I2C_Master_Start();
    I2C_Master_Write(I2C_add);
    I2C_Master_Write(data | LCD_BACKLIGHT);
    I2C_Master_Stop();
}