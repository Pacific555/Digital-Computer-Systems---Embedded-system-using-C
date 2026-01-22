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
  

  // LCD configuration
  LCD_DATA_WRITE &= ~0xFF;
  LCD_DATA_DIR |= 0xF0;    // P1.4-P1.7 To Output('1')
  LCD_DATA_SEL &= ~0xF0;   // Bit clear P2.4-P2.7
  LCD_CTL_SEL  &= ~0xE0;   // Bit clear P2.5-P2.7
  /////

 // P2.3 config
  TIMER_CAP_DIR  &= ~TIMER_CAP_LEG;
  TIMER_ST1_SEL  |= TIMER_CAP_LEG;
 // P2IE |= TIMER_CAP_LEG;


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
    state1_timer_init();
    state2_timer_init();
    state3_timer_init();
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
void ADCconfig(void){
    ADCCTL0 &= ~ENC ;
    ADCCTL1 |= SHP+CONSEQ_2;
    ADCCTL0 = SHT0_10 ;
    ADCMCTL0 |= INCH_3;  // INPUT ENTRY P1.3 (A3)
    ANALOG_INPUT |= 0X08;
}              
//-------------------------------------------------------------------------------------
//            Timer A0/A  configuration - For state2
//-------------------------------------------------------------------------------------
           
void state2_timer_init(void){
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    TIMER_ST2_CCTL0 = CCIE;
    TIMER_ST2_CCR0 = 0xFFFF;
    TIMER_ST2_CTL = SMCLK + MC_0 + ID_3;  //  select: 2 - SMCLK ; control: 3 - Up/Down  ; divider: 3 - /8      // Enter LPM0 w/ interrupt
}
//-------------------------------------------------------------------------------------
//            Timer A1/B configuration - For state1
//-------------------------------------------------------------------------------------
void state1_timer_init(void){
       TIMER_ST1_CTL = TBSSEL_2 + MC_0 + TBCLR; //+TBIE;
       TIMER_ST1_CCTL2 = CAP + CM_1  + SCS + CCIS_0; // Timer  configuration;
}

//-------------------------------------------------------------------------------------
//            Timer A1/B configuration - For state3
//-------------------------------------------------------------------------------------
void state3_timer_init(void){
    P2DIR |= 0X04;  // P2.2 CONFIG TO  output PWM
    P2SEL |= 0X04;
    TIMER_ST_3_CCTL1 = OUTMOD_7; // MODE 7
    TIMER_ST3_CTL |= TBSSEL_2;
    TIMER_ST3_CTL  &= 0Xffcf;
}




             
            
  

