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
extern void delay(unsigned int);
extern void delayHalfSecond();
extern void enterLPM(unsigned char);
extern void enable_interrupts();
extern void disable_interrupts();
extern void delayInMs(int); //delay in ms
extern void exitLpm();
extern void start_timer_state2();
extern void stop_timer_state2();
extern void  start_timer_st3(void);
extern void start_timer_state1(void);
extern void stop_timer_state1(void);
extern void return_values_state1(int * ,int * ,int * );
extern void enable_ADC_Interupts(void);
extern void disable_ADC_Interupts(void);
extern void start_sampling(void);
extern int return_reset_timer(void);
extern void  reset_timer_of(void);
extern void  reset_timer_on(void);
extern void stop_timer_st3();
extern int return_res();
extern void set_pwm_t(int, int);
extern void sleep_gie();
extern void start_timer_state4(void);
extern void stop_timer_st4(void);
extern __interrupt void PBs_handler(void);
extern __interrupt void Timer_handler(void);
extern __interrupt void TIMER1_A1_ISR(void);
extern __interrupt void ADC12_ISR(void);



#endif







