/* 
 * File:   main.c
 * Author: owenh
 *
 * Created on February 21, 2019, 12:35 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "uart.h"
#include "spi_setup.h"
#include "pin_mapping.h"
#include "timers.h"
#include "ads127.h"
#include "adc.h"
#include "lcd.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1) 
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1) 
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled) 
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed) 
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled) 
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled) 
 
 
// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1) 
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins) 
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq) 
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin) 
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled,
// Fail-Safe Clock Monitor is enabled) 

#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

 //Timer interrupt for heartbeat LED
void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt(void){
    _T1IF = 0;
    LATBbits.LATB8 ^= 1;
    true_rms(); //Don't actually put it here.. It's fucking slow rn.
    //adc_reg_to_uart(MODE_ADDR);
    //spi_write_reg(CONFIG_ADDR, 0x05);
    //spi_read_reg(CONFIG_ADDR);
}

////Timer interrupt for sampling frequency.
//void __attribute__((__interrupt__,__auto_psv__)) _IC1Interrupt(void){ 
//    //_T2IF = 0; 
//    _IC1IF = 0; 
//    uint16_t trash = IC1BUF;
//    //if(!ADC_DATA_READY_PORT){ //Check if data is ready
//    test_uart();
//        //get_data();   
//    //} else {
//    //    test_uart();
//    //}
//}

void setup(void){
    // Set default pin config
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;      //All pins are digital
    TRISA   = 0xfffe;
    TRISB   = 0x0003; //set Prog pins
    
    LATA = 0;
    LATB = 0;
    
    //Setup the ADC //Data ready pin is an input
    ADC_RESET_LAT = 1; 
    ADC_DATA_READY_TRIS = 1;
    
    LATBbits.LATB8 = 1;
    
    //Run setup functions
    init_timers();
    init_oc();
    init_ic(); 
    
    while(!_T1IF); //Allow for some time for the timers to boot up before starting the ADC. 
    
    spi_setup();
    uart_setup();
    lcd_init();
    
    // Enable interrupts
    _T1IE = 1;
}

int main(int argc, char** argv) {
    uint16_t trash;
    setup(); //Setup function
    //lcd_printStr("Hello");
    for(;;) {
        while(!_IC1IF);
        _IC1IF = 0;
        trash = IC1BUF;
        get_data();
    }
    return (EXIT_SUCCESS);
}

