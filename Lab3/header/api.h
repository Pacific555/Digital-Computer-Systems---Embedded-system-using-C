#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer


extern void lcd_puts(const char *);
extern void state1_func(char[33]);
extern void state2_func(unsigned int ,unsigned int ,  char[][M]  ,  char[][M]  );
extern void state3_func(int * );
void state4_func(char * src, char * dst, int blk_sz);

#endif







