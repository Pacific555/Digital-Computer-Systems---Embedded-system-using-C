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
  ///
  //LEDS
  LEDSSEL = 0X00;
  LEDSDIR = 0XFF;
  LEDSOUT = 0X00;

  // LCD configuration
  LCD_DATA_WRITE &= ~0xFF;
  LCD_DATA_DIR |= 0xF0;    // P1.4-P1.7 To Output('1')
  LCD_DATA_SEL &= ~0xF0;   // Bit clear P2.4-P2.7
  LCD_CTL_SEL  &= ~0xE0;   // Bit clear P2.5-P2.7
  /////

///KEYPAD_CONFIG
  KEYPAD_SEL = 0X00;
  KEYPAD_DIR = 0X0f;
  KEYPAD_OUT = 0X00;
  KEYPAD_IRQ_SEL &= ~KEYPAD_IRQ_INDEX;  // P2.1SEL = 0
  KEYPAD_IRQ_DIR &= ~KEYPAD_IRQ_INDEX;

  KEYPAD_IRQ_IES |= KEYPAD_IRQ_INDEX;
  KEYPAD_IRQ_IFG &= ~KEYPAD_IRQ_INDEX;


  // PushButtons Setup
  PBsArrPort = 0x00; // move 0 to p2in
  PBsArrPortSel &= ~0x8F;
  PBsArrPortDir &= ~0x0F;
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode
  PBsArrIntEn |= 0x0F;
  PBsArrIntPend &= ~0xFF;            // clear pending interrupts
 // PBsArrIntEn &= 0xF7;              // P1.3IFG changed to '1' , even when it's not connected
  _BIS_SR(GIE);                     // enable interrupts globally
}                             
//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){
    TACTL = TASSEL_2 + MC_0 +ID_3;                  // SMCLK/8, contmode, half a second
} 

//-------------------------------------------------------------------------------------
//            Timer A0/A  configuration - For state2
//-------------------------------------------------------------------------------------
           

//-------------------------------------------------------------------------------------
//            Timer A1/B configuration - For state1
//-------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------
//            Timer A1/B configuration - For state3
//-------------------------------------------------------------------------------------



///KEYPAD

   void Reset_Keypad(){
           KEYPAD_OUT  &= ~0x0F;
       }

            
 // void configDMA0    // for state

