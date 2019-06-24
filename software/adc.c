#include "adc.h"
#include "uart.h"
#include "lcd.h"

volatile float sampBuf[BUF_SZ];
volatile unsigned int wIdx = 0;     
volatile double v_rms = 0;

void get_data(){
    uint32_t data = spi_read_data();    
    float send = (float)data * (float)CONVERSION_CONST;
    send = (send - DC_OFFSET) / 176.6;
    send *= 1000;
    sampBuf[wIdx] = send;
    adc_data_to_uart((double)send);
    if(++wIdx == BUF_SZ){
        wIdx = 0;
    }
}


void true_rms(void){
    int i;
    double samp; 
    //_T2IE = 0; 
    for(i = 0; i < BUF_SZ; i++){
        samp = sampBuf[i]; //Convert the binary count into a fp number.
        //v_rms += pow((samp - DC_OFFSET) / (166.25), 2 ); //166.7
        v_rms += pow(samp, 2);
    }
    
    v_rms /= BUF_SZ;
    
    //v_rms = ((double) 5.99999983) * (sqrt(v_rms) - DC_OFFSET); 
    v_rms = sqrt(v_rms);
	//v_rms *= 1000; //Convert to mV
    //v_rms -= 0.3

    lcd_setCursor(0,0);
    lcd_printDouble(v_rms);
    //lcd_printStr("Hey");
    v_rms = 0; 
    //_T2IE = 1;
}
