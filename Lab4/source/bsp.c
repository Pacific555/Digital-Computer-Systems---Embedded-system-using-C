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
  P2SEL &= 0Xf8;     /// FOR RGB
  P2DIR |= 0X07;     /// FOR RGB
  P2OUT &= 0Xf8;     /// FOR RGB
  P2SEL &= ~ 0X08;
  P2DIR &= ~ 0X08;
  P2IES |= 0x08;
  P2IE  &= ~ 0X08;

  // LCD configuration
   LCD_DATA_WRITE &= ~0xFF;
   LCD_DATA_DIR |= 0xF0;    // P1.4-P1.7 To Output('1')
   LCD_DATA_SEL &= ~0xF0;   // Bit clear P2.4-P2.7
   LCD_CTL_SEL  &= ~0xE0;   // Bit clear P2.5-P2.7
   /////

   ////LDR leg 1.3 config
   P1SEL &= ~0x08;
   P1DIR &= ~0x08;
  
  _BIS_SR(GIE);                     // enable interrupts globally
}                             

void base_timer_config (void)
{
   TA0CTL = TASSEL_2 + MC_0;
   TA0CCR0 = 0X0418; // FOR 1 ms

}

void init_uart(){
    DCOCTL = 0;                               // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
    DCOCTL = CALDCO_1MHZ;

   // P2DIR = 0xFF;                             // All P2.x outputs
   // P2OUT = 0;                                // All P2.x reset
    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  // P1DIR |= RXLED + TXLED;
   // P1OUT &= 0x00;

    UCA0CTL1 |= UCSSEL_2;                     // CLK = SMCLK
    UCA0BR0 = 104;                           //
    UCA0BR1 = 0x00;                           //
    UCA0MCTL = UCBRS0;               //
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

void PWM_for_bazzer_config(void){
    P2DIR |= 0X10;  // P2.4 CONFIG TO  output PWM
    P2SEL |= 0X10;
    TA1CCTL2 = OUTMOD_7; // MODE 7
    TA1CTL |= TASSEL_2; //SBMCLK
    TA1CTL  &= 0Xffcf; // MC0
}

void ADC_config(){

      ADC10CTL1 = CONSEQ_2 + INCH_3;            // Repeat single channel, A1
      ADC10CTL0 = ADC10SHT_2 + MSC; //+ ADC10ON + ADC10IE; // ADC10ON, interrupt enabl
      ADC10AE0 |= 0x08;
}


 
             

