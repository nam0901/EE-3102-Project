/*
 * File:   lcd.c
 * Author: AlexM
 *
 * Created on March 28, 2018, 10:10 AM
 * 
 * 
 */


#include "xc.h"
#include "lcd.h"
#include <stdio.h>

#define CONTRAST 0b010111

void lcd_cmd(char package)
{
    
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN);
    asm("nop");
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01111100;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b00000000;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = package;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN);
    asm("nop");
    IFS3bits.MI2C2IF = 0;
    
}

void lcd_init()
{
    
    //set up SDA2 and SCL2 as outputs
    _TRISB2 = 0;
    _TRISB3 = 0;
    
    IFS3bits.MI2C2IF = 0;
    I2C2CON = 0;
    I2C2BRG = 157;   //set clk to 100kHz
    I2C2CONbits.I2CEN = 1;
    
    //setup tmr2 for setup delays (50ms)
    T4CON = 0x0020;
    TMR4 = 0;
    _T4IF = 0;
    PR4 = 12499;
    T4CONbits.TON = 1;
    
    while(_T4IF == 0);
    _T4IF = 0;
    
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000 + (CONTRAST & 0b001111)); // contrast C3-C0
    lcd_cmd(0b01011100 + (CONTRAST >> 4)); // C5-C4, Ion, Bon
    lcd_cmd(0b01101100); // follower control
    
    int i = 0;
    for(i = 0; i < 5; i++)
    {
        while(_T4IF == 0);
        _T4IF = 0;
    }
    i = 0;
    
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    
    while(_T4IF == 0);
    _T4IF = 0;
}

void lcd_setCursor(char x, char y)
{
    char location = 0x40*(y) + x + 0x80;
    lcd_cmd(location);
    
}

void lcd_printChar(char package)
{
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN);
    asm("nop");
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01000000; // 8-bits consisting of control byte /w RS=1
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = package; // 8-bits consisting of the data byte
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN); // PEN will clear when Stop bit is complete
    asm("nop");
    IFS3bits.MI2C2IF = 0;

}

void lcd_printStr(const char *s)
{
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN);
    asm("nop");
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
        
    I2C2TRN = 0b01000000; // 8-bits consisting of control byte /w RS=1
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    
    int i = 0;
    while(s[i] != 0)    //loop to print out all the characters in the string
    {
        I2C2TRN = s[i]; 
        while(IFS3bits.MI2C2IF == 0);
        IFS3bits.MI2C2IF = 0;
        i++;
    }

    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN); // PEN will clear when Stop bit is complete
    asm("nop");
    IFS3bits.MI2C2IF = 0;
}

void lcd_printDouble(double data) 
{
    char buffer[50];
    sprintf(buffer, "%f", data);
    lcd_printStr(buffer);
}
