#ifndef _bsp_H_
#define _bsp_H_


 #include  <msp430xG46x.h>  // MSP430x4xx


#define   debounceVal      250
#define   debounceValKP    3000
#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms
#define   upTime           18
#define   downTime         3
#define   NAME_SHOW_RATE   0x79FF



// LEDs abstraction
#define LEDsArrPort        P9OUT
#define LEDsArrPortDir     P9DIR
#define LEDsArrPortSel     P9SEL
/*
// Switches abstraction
#define SWsArrPort         P2IN
#define SWsArrPortDir      P2DIR
#define SWsArrPortSel      P2SEL
#define SWmask             0x0F
*/
// PushButtons abstraction
#define PBsArrPort	       P1IN
#define PBsArrIntPend	   P1IFG
#define PBsArrIntEn	       P1IE
#define PBsArrIntEdgeSel   P1IES
#define PBsArrPortSel      P1SEL
#define PBsArrPortDir      P1DIR

//    LCD abstraction

#define LCD_DATA_WRITE    P1OUT
#define LCD_DATA_DIR      P1DIR
#define LCD_DATA_SEL      P1SEL
#define LCD_CTL_SEL       P2SEL
#define LCD_DATA_READ     P1IN 

#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08

//      key pad

#define KEY_PAD_DIR       P10DIR
#define KEY_PAD_IN        P10IN
#define KEY_PAD_OUT       P10OUT

#define KeyPadArrPort         P2IN
#define KeyPadArrIntPend      P2IFG
#define KeyPadArrIntEn        P2IE
#define KeyPadArrIntEdgeSel   P2IES
#define KeyPadArrPortSel      P2SEL
#define KeyPadArrPortDir      P2DIR

#define KEY_PAD_PRESS     0x02

extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);
extern void DMAconfig(void);
extern char data_matrix_in[10][36];
extern char data_matrix_out[10][36];
extern int ledsValue;
#endif
