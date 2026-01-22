#ifndef _api_H_
#define _api_H_

#include  "../header/halGPIO.h"     // private library - HAL layer



extern void ShiftNum2LEDs( unsigned int  );
extern void generatePwm(void);
extern void printDigitArr2LEDs(char[] , int , unsigned int  );
extern void PrintStr(char[]  , int );
#endif







