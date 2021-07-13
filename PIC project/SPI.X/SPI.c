/*
 * File:   SPI.c
 * Author: DELL
 *
 * Created on May 3, 2021, 3:40 PM
 */

#include <pic16f887.h>

#include "SPI.h"

void SPI_init(SPI_mode mode, SPI_dataSample sample, SPI_clockIdle idle, SPI_transmitEdge edge) {
    TRISC4 = 1;
    TRISC5 = 0;
    if (mode & 0b00000100) // slave
    {
        TRISC3 = 1;
        SSPSTAT = edge;
    } else // master
    {
        TRISC3 = 0;
        SSPSTAT = sample | edge;
        
    }
    SSPCON = mode | idle;
}

unsigned char SPI_transfer(unsigned char data)
{
    SSPBUF = data;
    while(!SSPSTATbits.BF);
    return SSPBUF;
}