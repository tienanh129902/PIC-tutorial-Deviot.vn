/*
 * File:   main.c
 * Author: DELL
 *
 * Created on April 1, 2021, 3:49 PM
 */



#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "lcd.h"
#include "uart.h"
#include <string.h>

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = 256        // Flash Program Memory Write Enable bits (0000h to 00FFh write-protected; 0100h to 1FFFh may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


/*
 Program Flow related definition
 */
#define DHT11_Data_Pin   PORTDbits.RD5
#define DHT11_Data_Pin_Direction  TRISDbits.TRISD5


#define _XTAL_FREQ 20000000 //20 Mhz

char buffer[30], value[20];
char receivedTemp_int, receivedTemp_dec, receivedHumid_int, receivedHumid_dec;
unsigned char Check_bit, Temp_byte_1, Temp_byte_2, RH_byte_1, RH_byte_2;
unsigned char humidity, RH, checksum;

//Dht11 related definition

void DHT11_init();
void DHT11_checkResponse();
char DHT11_read();

//void lim_parse(char *str) {
//    if (!strncmp(str, "Limited temp: ", 14)) {
//        if (sscanf(str, "Limited temp: %d.%d"), &receivedTemp_int, &receivedTemp_dec) {
//            FLASH_WRITE(0, receivedTemp_int);
//            return;
//        }
//    } else if (!strncmp(str, "Limited Humid: ", 15)) {
//        if (sscanf(str, "Limited Humid: %d.%d"), &receivedHumid_int, &receivedTemp_dec) {
//            FLASH_WRITE(2, receivedHumid_int);
//            return;
//        }
//    }
//}
// System related definitions
//char limTemp = __flash_read(0);
//char limHumid = __flash_read(2);
void system_init(void);
void introduction(void);
void clear(void);

void main() {
    system_init();
    TRISA0 = 0;
    UART_init(9600);
    while (1) {
        __delay_ms(800);
        DHT11_init();
        DHT11_checkResponse();
        if (Check_bit == 1) {
            RH_byte_1 = DHT11_read();
            RH_byte_2 = DHT11_read();
            Temp_byte_1 = DHT11_read();
            Temp_byte_2 = DHT11_read();
            checksum = DHT11_read();

            if (checksum == ((RH_byte_1 + RH_byte_2 + Temp_byte_1 + Temp_byte_2) & 0XFF)) {
                humidity = Temp_byte_1;
                RH = RH_byte_1;
                lcd_com(FIRST_LINE);
                sprintf()
                lcd_puts("Temp: ");
//                lcd_data(0xDF);
//                lcd_puts("C");
                lcd_com(SECOND_LINE);
                lcd_puts("Humidity: ");
                lcd_data(48 + ((RH / 10) % 10));
                lcd_data(48 + (RH % 10));
//                lcd_puts("%  ");
                //                sprintf(value, "Temp: %d.%d ", Temp_byte_1, Temp_byte_2);
                //                UART_sendString(value);
                //                UART_sendString("\n");
                //                sprintf(value, "Humid: %d.%d %", RH_byte_1, RH_byte_2);
                //                UART_sendString(value);
                //                UART_sendString("\n");
                
                do {
                    UART_getString(buffer, 30);
                    lim_parse(buffer);
                }while(1);
//                if (RH_byte_1 >= limHumid || Temp_byte_1 >= limTemp) {
//                    RA0 = 1;
//                }
            }
            else {
                lcd_puts("Check sum error");
            }
        } else {
            clear();
            lcd_com(FIRST_LINE);
            lcd_puts("Error!!!");
            lcd_com(SECOND_LINE);
            lcd_puts("No Response.");
        }
        __delay_ms(1000);
    }

}

void DHT11_init() {
    DHT11_Data_Pin_Direction = 0; //Configure RD0 as output
    DHT11_Data_Pin = 0; //RD0 sends 0 to the sensor
    __delay_ms(18);
    DHT11_Data_Pin = 1; //RD0 sends 1 to the sensor
    __delay_us(30);
    DHT11_Data_Pin_Direction = 1; //Configure RD0 as input
}

void DHT11_checkResponse() {
    Check_bit = 0;
    __delay_us(40
            );
    if (DHT11_Data_Pin == 0) {
        __delay_us(80);
        if (DHT11_Data_Pin == 1) {
            Check_bit = 1;
        }
        __delay_us(50);
    }
}

char DHT11_read() {
    char data, for_count;
    for (for_count = 0; for_count < 8; for_count++) {
        while (!DHT11_Data_Pin);
        __delay_us(30);
        if (DHT11_Data_Pin == 0) {
            data &= ~(1 << (7 - for_count)); //Clear bit (7-b)
        } else {
            data |= (1 << (7 - for_count)); //Set bit (7-b)
            while (DHT11_Data_Pin);
        } //Wait until PORTD.F0 goes LOW
    }
    return data;
}

void system_init() {
    TRISB = 0; // LCD pins set to out. 
    lcd_init();
    introduction();
}

void clear(void) {
    lcd_com(FIRST_LINE);
    lcd_puts("                ");
    lcd_com(SECOND_LINE);
    lcd_puts("                ");
}

void introduction(void) {
    lcd_com(FIRST_LINE);
    lcd_puts("Deviot.vn");
    lcd_com(SECOND_LINE);
    lcd_puts("Project class");
    __delay_ms(1000);
    __delay_ms(1000);
    clear();
    lcd_com(FIRST_LINE);
    lcd_puts("DHT11 sensor");
    lcd_com(SECOND_LINE);
    lcd_puts("with PIC16F877A");
    __delay_ms(1000);
    __delay_ms(1000);
}

