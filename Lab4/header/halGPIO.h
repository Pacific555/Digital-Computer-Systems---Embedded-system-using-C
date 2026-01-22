#ifndef _halGPIO_H_
#define _halGPIO_H_
#include <stdio.h>
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
extern void delay(unsigned int);
extern void delayHalfSecond();
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void delayInMs(int); //delay in ms
extern void exitLpm();
extern void start_timer();
extern void stop_timer();
extern int get_x();
extern int get_num();
extern void define_RGB_color(char);
extern void sleep_gie();
extern void set_pwm_t(int);
extern void  start_timer_st3();
extern void  stop_timer_st3();
extern void setX(int);
extern unsigned int get_res();
extern void enable_ADC_Interupts();
extern void disable_ADC_Interupts();
extern void start_sampling();
extern void enable_button();
extern void  disable_button();
extern void send_str(char[16]);

extern __interrupt void Timer_handler (void);
extern __interrupt void USCI0RX_ISR(void);
extern __interrupt void ADC10_ISR(void);
extern __interrupt void PBs_handler(void);
#endif







