/* 
 * File:   timers.h
 * Author: owenh
 *
 * Created on March 14, 2019, 3:13 PM
 */

#ifndef TIMERS_H
#define	TIMERS_H

#include <xc.h>

#define CLK_OC_PR 2
    
void init_oc();
void init_ic(); 
void init_timers();

#endif	/* TIMERS_H */
