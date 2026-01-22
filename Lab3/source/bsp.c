//#ifdef __MSP430FG4619
#include <header/bsp_430FG4619.h>
//#else
//#include  "../header/bsp_430G2553.h"
//#endif
//-----------------------------------------------------------------------------  
//           GPIO congiguration
//-----------------------------------------------------------------------------
char data_matrix_in[10][36] = {
                                 "An apple a day keeps the doctor away",
                                 "climb on the bandwagon",
                                 "Dot the i's and cross the t's",
                                 "He who pays the piper calls the tune",
                                 "The pen is mightier than the sword",
                                 "The pot calling the kettle black",
                                 "shed crocodile tears",
                                 "Close but no cigar",
                                 "Cut from the same cloth",
                                 "Strike while the iron’s hot"
 };

int ledsValue = 0x01;
char data_matrix_out[10][36];
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;		// Stop WDT
   
  // LEDs 8-bit Array Port configuration
  LEDsArrPortSel &= ~0xFF;            // GPIO capability
  LEDsArrPortDir |= 0xFF;             // output dir
  LEDsArrPort = 0x00;                 // clear all LEDs

  // PushButtons Setup
  PBsArrPortSel &= ~0x0f;
  PBsArrPortDir &= ~0x0f;
  PBsArrIntEdgeSel |= 0x03;  	     // pull-up mode
  PBsArrIntEdgeSel &= ~0x0c;         // pull-down mode
  PBsArrIntEn |= 0x0f;
  PBsArrIntPend &= ~0x0f;            // clear pending interrupts
  
/*  // PushButtons Setup
  PB3ArrPortSel &= ~0x01;
  PB3ArrPortDir &= ~0x01;
  PB3ArrIntEdgeSel &= ~0x01;         // pull-down mode
  PB3ArrIntEn |= 0x01;
  PB3ArrIntPend &= ~0x01;            // clear pending interrupts 
*/

/*
  
  //genrator Setup 
  GENsArrPortSel |= 0x10;
  GENsArrPortDir &= ~0x10;
  
  //buzer Setup 
   BUZER_OUT     &= ~0x04;
   BUZER_DIR     |= 0x04;
   BUZER_SEL     |= 0x04;
*/
  //LCD Setup 
   LCD_DATA_WRITE      &= ~0xFF;
   LCD_DATA_DIR        |= 0xF0;
   LCD_DATA_SEL        &= ~0xF0;
   LCD_CTL_SEL         &= ~0xE0;
  
   //key pad Setup
   KEY_PAD_DIR          = 0x0f;
   KEY_PAD_OUT          = 0x00;
   KeyPadArrPortSel     &= ~0x02;
   KeyPadArrPortDir     &= ~0x02;
   KeyPadArrIntEdgeSel  |= 0x02;
   KeyPadArrIntPend     &= ~0x02;

  _BIS_SR(GIE);                     // enable interrupts globally
}



//------------------------------------------------------------------------------------- 
//            Timers congiguration 
//-------------------------------------------------------------------------------------
void TIMERconfig(void){

        TBCTL = TBSSEL_1 + ID_2 + TBCLR; //ACLK/4 = 2^13 +count up to 0xffff +clear
//        TA0CTL = TASSEL_2+ TACLR +ID_3;  // timer is off SMCLK /8 = 0.5 sec interupt
        TB0CCTL0 = CCIE;    //interupt enable
        TB0CCR0 = 0x0FFF;      // value of 0.5 sec = 2^12/2^13
        /*
        TA1CTL = TASSEL_2 +TACLR; 
        TA1CCTL1 = OUTMOD_0;   // for state3
        TA1CCTL2 = CM_1 + SCS + CAP;     // for state1 
        */
} 
//------------------------------------------------------------------------------------- 
//            ADC congiguration 
//-------------------------------------------------------------------------------------
/*
void ADCconfig(void){
	
	    ADC10CTL0 &= ~0x0002;// enc off
        ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE;   //set ref 0-3v, sample rate= 8*(1/2^20)
        ADC10CTL1 = INCH_3+ADC10SSEL_3; //A3,SMCLK
        ADC10AE0 |= BIT3;
}              

           
*/
void DMAconfig(void){
       DMA0SZ = 0x024;
       DMACTL0 = DMA0TSEL_0 + DMA1TSEL_8; // first dma turnes on manually and secound turnes on by timer b compare
       DMA0CTL = DMADT_1 + DMASBDB + DMAEN + DMAIE + DMADSTINCR_3 + DMASRCINCR_2; // block mode , byte-to-byte , enable

       DMA1SZ = 0x001;
       DMA1CTL = DMADT_0 | DMAIE;                     // Single transfer mode, interrupt enabled
       DMA1DA = (unsigned int)&LEDsArrPort;           // Destination: LEDs

       DMA2SZ = 0x09E;
       DMA2CTL = DMADT_1 + DMASBDB + DMAEN + DMAIE + DMADSTINCR_3 + DMASRCINCR_2; // block mode , byte-to-byte , enable

}
 
             
             
            
  

