#ifndef _bsp_H_4xx_
#define _bsp_H_4xx_

#include  <msp430xG46x.h>  // MSP430x4xx    (MSP430FG4619)


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
#define LCD_CTL_SEL         P2SEL
#define LCD_DATA_SEL      P1SEL


// timers

#define SMCLK  TASSEL_2
#define SMCLK_ST1  TBSSEL_2

#define TIMER_ST2_VEC TIMER0_A0_VECTOR
#define TIMER_ST2_CCTL0  TACCTL0
#define TIMER_ST2_CCR0 TACCR0
#define TIMER_ST2_CTL     TACTL
#define TIMER_ST2_RESET    TASSEL_2 + MC_0 + ID_3;

#define TIMER_ST1_CCTL2 TBCCTL2
#define TIMER_ST1_CTL TBCTL
#define TIMER_ST1_CCR2 TBCCR2
#define TIMER_ST1_START TBSSEL_2 + MC_2 + TBCLR
#define TIMER_ST1_STOP TBSSEL_2 + MC_0 + TBCLR

#define TIMER_CAP_LEG 0x08
#define TIMER_CAP_IN    P2IN
#define TIMER_CAP_DIR    P2DIR
#define TIMER_ST1_SEL   P2SEL
#define VECTOR_TIMER_ST1_CCR2 TBIV_TBCCR2
#define VECTOR_TIMER_ST1_CCR1 TBIV_TBCCR1
#define VECTOR_TIMER_ST1_IFG TBIV_TBIFG
#define VECTOR_TIMER_ST1 TBIV
#define TIMER_ST1_VEC    TIMER0_B1_VECTOR
#define TIMER_ST13_ISR_VEC   TBIV
#define TIMER_ST1_ISR      TBIV_TBCCR2


extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);
extern void state2_timer_init(void);
extern void state1_timer_init(void);
#endif





