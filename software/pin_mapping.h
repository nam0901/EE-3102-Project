/* 
 * File:   pin_mapping.h
 * Author: owenh
 *
 * Created on March 14, 2019, 3:17 PM
 */

#ifndef PIN_MAPPING_H
#define	PIN_MAPPING_H

//For any pins that are directly driven by the MCU, define them in this file. 

//SPI LAT
#define SPI_CS_LAT LATBbits.LATB11 //PIN 22

//SPI TRIS
#define SPI_DOUT_TRIS TRISBbits.TRISB13 //PIN 24
#define SPI_DIN_TRIS TRISBbits.TRISB14 //PIN 25
#define SPI_CS_TRIS TRISBbits.TRISB11 //PIN 22
#define SPI_SCLK_TRIS TRISBbits.TRISB12 //PIN 23

//ADC LAT
#define ADC_RESET_LAT LATBbits.LATB9 //Pin 18

//ADC PORT
#define ADC_DATA_READY_PORT PORTBbits.RB15 //Pin 26

//ADC TRIS
#define ADC_RESET_TRIS TRISBbits.TRISB9 //Pin 18
#define ADC_DATA_READY_TRIS TRISBbits.TRISB15 //Pin 26

#endif

