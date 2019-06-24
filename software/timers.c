/* 
 * File:   timers.c
 * Author: owenh
 *
 * Created on February 21, 2019, 2:15 PM
 */
#include "timers.h"

//Clocks needed (some clocks can be derived from the same timer):
//CLK
//SCLK
//I2C and UART are generated by their respective peripherals
//Misc timer for data processing?

void init_oc(void){
    //Init CLK OC
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR5bits.RP10R = 18;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
    //Configure OC1
    OC1CON = 0; 
    OC1R = CLK_OC_PR;
    OC1RS = CLK_OC_PR; 
    OC1CONbits.OCTSEL = 1; //Use Timer 3
    OC1CONbits.OCM = 0b110; // Output compare PWM w/o faults
}

void init_ic(void){
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 15;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
    IC1CON = 0; //It's cool for it to use TMR2
    _IC1IF = 0; 
    //_IC1IE = 1; 
    IC1CONbits.ICM = 0b010; 
}

void init_timers(void){
    
    //Init timer 1 for 1 second heartbeat LED
    T1CON = 0x0030; //set PRE to 256
    PR1 = 42499;
    TMR1 = 0;
    _T1IF = 0;
    //_T1IE = 1; Moved to end of setup to allow LCD to setup before we try and send things
    T1CONbits.TON = 1;
    
    T2CON = 0x0000; //Prescaler of zero can be used
    PR2 = 799;      //was 799//Should give 20kHz timer
    TMR2 = 0;
    _T2IF = 0;
    //_T2IE = 1;
    T2CONbits.TON = 1;
    
    //Init timer 3 for 8MHz CLK
    T3CON = 0x0000; //Keep timer 3 off, 1:8 prescalar. TCKPS<1:0> : Prescaler 1,8,64,256 (divides input clock to counter)
    PR3 = CLK_OC_PR;
    TMR3 = 0; //Reset the internal timer register. 
    _T3IF = 0;
    T3CONbits.TON = 1; //Turn the timer on.
    
    /*
     * NOTE: Using Timer4 for LCD setup. Please see lcd.c for more info.
     */
    
}