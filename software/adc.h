/* 
 * File:   adc.h
 * Author: owenh
 *
 * Created on April 11, 2019, 4:03 PM
 */

#ifndef ADC_H
#define	ADC_H

#include "xc.h"
#include "ads127.h"
#include "math.h"
#include "uart.h"
#include "spi_setup.h"
#define BUF_SZ 1024  //used to be 1024
#define DC_OFFSET ((double) 1.549020) //used to be 1.549020

void true_rms(void);
void get_data();

#endif	/* ADC_H */

