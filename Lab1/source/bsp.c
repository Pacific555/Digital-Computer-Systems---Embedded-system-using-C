  // private library - BSP layer
#ifdef __MSP430FG4619__
#include "../header/bsp_msp430x4xx.h"
#define LabKit
#else
#include "../header/bsp_msp430x2xx.h"
#define PersonalEvalKit
#endif
//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  // LEDs 8-bit Array Port configuration
  LEDsArrPortSel &= ~0xFF;            // GPIO capability
  LEDsArrPortDir |= 0xFF;             // output dir
  LEDsArrPort = 0x00;				  // clear all LEDs
  

  // PushButtons Setup
  PBsArrPort = 0x00; // move 0 to p2in
  PBsArrPortSel &= ~0x8F;
  PwmOutPort    |= 0x80;   // for PWM
  PBsArrPortDir &= ~0x0F;
  PBsArrPortDir  |= 0x80;   // for PWM
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode
  PBsArrIntEn |= 0x0F;
  PBsArrIntPend &= ~0x8F;            // clear pending interrupts
  
  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
	
	//write here timers congiguration code
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
	
	//write here ADC congiguration code
}              

           
             

 
             
             
            
  

