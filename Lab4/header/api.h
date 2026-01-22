#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern void lcd_puts(const char * );
extern unsigned int get_frac_from_Q(unsigned int Q );
extern unsigned int get_int_from_Q(unsigned int Q );
extern void state1_func();
extern void state2_func();
extern void state3_func();
extern void state4_func();
extern void state5_func();
extern void state6_func();
extern void state7_func();
#endif







