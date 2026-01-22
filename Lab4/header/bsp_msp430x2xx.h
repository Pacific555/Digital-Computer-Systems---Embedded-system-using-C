#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx

#define   debounceVal      250




//LCD
#define LCD_DATA_WRITE  P1OUT
#define LCD_DATA_DIR    P1DIR
#define LCD_DATA_READ   P1IN
#define LcdCtrl             P2OUT
#define LcdCtrlDir          P2DIR
#define LcdCtrl             P2OUT
#define TIMER_CAP_IN    P2IN
#define TIMER_CAP_DIR    P2DIR
#define LCD_CTL_SEL         P2SEL
#define LCD_DATA_SEL      P1SEL
#define TIMER_CAP_LEG 0x10

#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1


#define PB1                0x08

extern void GPIOconfig(void);
extern void base_timer_config (void);
extern void PWM_for_bazzer_config(void);
extern void init_uart();
extern void ADC_config();
#endif



