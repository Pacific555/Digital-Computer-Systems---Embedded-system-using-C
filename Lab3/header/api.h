#ifndef _api_H_
#define _api_H_

#include <string.h>
#include "halGPIO.h"     // private library - HAL layer

extern char idiom_recorder[32];
extern char str_swap[158];
extern char str[158];

//extern void findHz();
//extern void NumToString(int num);
extern void UpDownCount();
extern void timerBOnfor1sec();
extern void timerBOnforhalfsec();
extern void idiom_record();
extern void lineSwap();
extern void printLcdDOWN(char *toPrint);
extern void printLcdUP(char *toPrint);
extern void Right2LeftLeds();
extern void Swap_String();
extern void mirror_string();
extern void Print_Arr_on_Leds();
extern void swap_half_string();
extern char* dma_strSwap(char** data, int idx, char* strout);
extern void swap_half_string2();

//extern void buzzer_gen();
//extern void Real_Time(char* first1,char* first2,char* last1,char* last2);
//extern void twosectimecount();

#endif
