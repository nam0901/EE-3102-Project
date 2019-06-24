#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.  

void uart_setup(void);
void test_uart(void);
void adc_reg_to_uart(uint8_t addr);
void adc_data_to_uart(double adc_data);

#endif

