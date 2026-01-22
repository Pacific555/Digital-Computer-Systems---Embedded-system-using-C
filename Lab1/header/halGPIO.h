#ifndef _halGPIO_H_
#define _halGPIO_H_

#include  "../header/app.h"    		// private library - APP layer
#ifdef __MSP430FG4619__
#include "bsp_msp430x4xx.h"
#else
#include "bsp_msp430x2xx.h"
#endif
//#include "bsp_msp430x4xx.h"    		// private library - BSP layer


extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

extern void sysConfig(void);
extern void print2LEDs(unsigned char);
extern void clrLEDs(void);
extern void setLEDs(char);
extern unsigned char readSWs(void);
extern void delay(unsigned int);
extern void delayHalfSecond();
extern void clearPWM();
extern void enterLPM(unsigned char);
extern void incLEDs(char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void printDigit2LEDs(unsigned char ch);
extern char shiftLEDs(void);
extern void toggeleBit(void);
extern void delayInMs(int); //delay in ms
extern __interrupt void PBs_handler(void);


#endif







