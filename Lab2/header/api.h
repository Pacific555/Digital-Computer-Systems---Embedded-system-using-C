#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer

extern void freqCount();
extern void countMinute();
extern void UpdateDigits(char * , char * , char * , char* );
extern void PrintTime(char * , char * , char * );
extern void printFreq(unsigned int  );
extern void moveToFreq();
extern void  tone_gen();
extern int deterNumSamples(int );
extern void lcd_puts(const char * s);
extern void print_names(char * , char * , char * , char* );
#endif







