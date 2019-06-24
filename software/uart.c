/*
 * File:   uart.c
 * Author: owenh
 *
 * Created on February 21, 2019, 12:40 PM
 */

#include <stdio.h>
#include "uart.h"
#include "spi_setup.h"
#include "ads127.h"

void uart_setup(void){
    __builtin_write_OSCCONL(OSCCON & 0xBF); //Unlock PPS
        RPINR18bits.U1RXR = 6;//RX: PIN 15 -> RP6 on PIC (was pin 2 but this doesn't have PPS). This is the Green wire on the dongle.
        RPOR3bits.RP7R = 3; //TX: PIN 16 -> RP 7 on PIC (was pin 2 but this doesn't have PPS). This is the White wire on the dongle.
    __builtin_write_OSCCONL(OSCCON | 0x40); //Lock PPS
    
    //Configure UART 1
    U1BRG = 1; //Baud = 500kbps
    U1MODE = 0x8000; //Enable uart for 8 bit data
    U1STA = 0;
    U1STAbits.UTXEN = 1; //Enable UART1 TX.
    U1MODEbits.UARTEN = 1; //Enable UART1
    
    int delay = 65535;
    while(delay--);
    
    //Enable the uart interrupt
}

void write_uart(uint8_t data){
    U1TXREG = data; 
    while(!U1STAbits.TRMT); //Wait for the transmission to complete. TODO: Make this nonblocking
}

void print_uart_string(char * str){
    while(*str != '\0'){
        write_uart(*str);
        str++;
    }  
}

void adc_reg_to_uart(uint8_t addr){
    char buffer[30];
    uint8_t response = spi_read_reg(addr);
    sprintf(buffer, "SPI Data: %02X\n", response);
    print_uart_string(buffer);
}

void adc_data_to_uart(double adc_data){
    char buffer[50];
    //uint32_t data = spi_read_data();
    
    //double d_data = (double) data; //Convert the binary count into a fp number.
    //d_data *= CONVERSION_CONST; 
    //DO ADC FORMAT CONVERSION HERE!
    
    //sprintf(buffer, "Timestamp: %d. ADC Reading: %f\n", TMR1, adc_data);
    sprintf(buffer, "%f\n", adc_data);
    print_uart_string(buffer);
}

void test_uart(void){
    print_uart_string("Hello World!\n");
}
