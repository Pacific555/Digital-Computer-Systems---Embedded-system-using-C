#ifndef _bsp_H_4xx_
#define _bsp_H_4xx_

#include  <msp430xG46x.h>  // MSP430x4xx    (MSP430FG4619)


#define   debounceVal      250
#define   LEDs_SHOW_RATE  500 //  0x80000  // 62_5ms
#define TPwm 20

// LEDs abstraction
#define LEDsArrPort        P9OUT   //Lab's MSP
#define LEDsArrPortDir     P9DIR
#define LEDsArrPortSel     P9SEL

// Switches abstraction
#define SWsArrPort         P2IN
#define SWsArrPortDir      P2DIR
#define SWsArrPortSel      P2SEL
#define SWmask             0x0F

// PushButtons abstraction
#define PBsArrPort	   P2IN 
#define PwmOutPort      P2OUT
#define PBsArrIntPend	   P2IFG 
#define PBsArrIntEn	   P2IE
#define PBsArrIntEdgeSel   P2IES
#define PBsArrPortSel      P2SEL 
#define PBsArrPortDir      P2DIR 
#define PB0                0x01
#define PB1                0x02
#define PB2                0x04
#define PB3                0x08


#define delay_0 3
#define delay_1 18
extern void GPIOconfig(void);
extern void TIMERconfig(void);
extern void ADCconfig(void);

#endif



