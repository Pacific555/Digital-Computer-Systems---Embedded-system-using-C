#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx

#define   debounceVal      250

// PushButtons abstraction
#define PBsArrPort     P1IN
#define PBsArrIntPend      P1IFG
#define PBsArrIntEn    P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL
#define PBsArrPortDir      P1DIR
#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08




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


// timers

#define SMCLK  TASSEL_2
#define SMCLK_ST1  TASSEL_2

#define TIMER_ST2_VEC TIMER0_A0_VECTOR
#define TIMER_ST2_CCTL0  TACCTL0
#define TIMER_ST2_CCR0 TACCR0
#define TIMER_ST2_CTL     TACTL
#define TIMER_ST2_RESET    TASSEL_2 + MC_0 + ID_3;

#define TIMER_ST1_CCTL2 TA1CCTL2
#define TIMER_ST1_CTL TA1CTL
#define TIMER_ST3_CTL TA1CTL
#define TIMER_ST_3_CCTL1 TA1CCTL1
#define TIMER_ST_3_CCR1 TA1CCR1
#define TIMER_ST_3_CCR0 TA1CCR0
#define TIMER_ST1_CCR2 TA1CCR2
#define TIMER_ST1_START TASSEL_2 + MC_2 + TACLR
#define TIMER_ST1_STOP TASSEL_2 + MC_0 + TACLR
#define TIMER_ST1_IFG TAIFG

#define TIMER_CAP_LEG 0x08
#define TIMER_CAP_IN    P2IN
#define TIMER_CAP_DIR    P2DIR
#define TIMER_ST1_SEL   P2SEL
#define VECTOR_TIMER_ST1_CCR2 TA1IV_TA1CCR2
#define VECTOR_TIMER_ST1_CCR1 TA1IV_TA1CCR1
#define VECTOR_TIMER_ST1_IFG TA1IV_TA1IFG
#define VECTOR_TIMER_ST1 TA1IV
#define TIMER_ST1_VEC    TIMER0_A1_VECTOR
#define TIMER_ST13_ISR_VEC   TA1IV
#define TIMER_ST1_ISR      TA1IV_TACCR2


#define ANALOG_INPUT P1SEL

extern void GPIOconfig(void);
extern void TIMERconfig(void);




#endif



