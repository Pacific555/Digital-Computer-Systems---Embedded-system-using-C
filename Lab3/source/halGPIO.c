#include <header/halGPIO.h>
#include "string.h"   // private library - HAL layer

//global var
unsigned int cycle = 0;
//unsigned int start;
//unsigned int end;


//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	TIMERconfig();
	DMAconfig();

	//ADCconfig();

}
//******************************************************************
// initialize the LCD
//******************************************************************
void lcd_init(){
  
	char init_value;

	if (LCD_MODE == FOURBIT_MODE) init_value = 0x3 << LCD_DATA_OFFSET;
        else init_value = 0x3F;
	
	LCD_RS_DIR(OUTPUT_PIN);
	LCD_EN_DIR(OUTPUT_PIN);
	LCD_RW_DIR(OUTPUT_PIN);
        LCD_DATA_DIR |= OUTPUT_DATA;
        LCD_RS(0);
	LCD_EN(0);
	LCD_RW(0);
        
	DelayMs(15);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayMs(5);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	DelayUs(200);
        LCD_DATA_WRITE &= ~OUTPUT_DATA;
	LCD_DATA_WRITE |= init_value;
	lcd_strobe();
	
	if (LCD_MODE == FOURBIT_MODE){
		LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= 0x2 << LCD_DATA_OFFSET; // Set 4-bit mode
		lcd_strobe();
		lcd_cmd(0x28); // Function Set
	}
        else lcd_cmd(0x3C); // 8bit,two lines,5x10 dots 
	
	lcd_cmd(0xF); //Display On, Cursor On, Cursor Blink
	lcd_cmd(0x1); //Display Clear
	lcd_cmd(0x6); //Entry Mode
	lcd_cmd(0x80); //Initialize DDRAM address to zero
        enable_interrupts();
        
}
//******************************************************************
// send a command to the LCD
//******************************************************************
void lcd_cmd(unsigned char c){
  
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	if (LCD_MODE == FOURBIT_MODE)
	{
		LCD_DATA_WRITE &= ~OUTPUT_DATA;// clear bits before new write
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;
		lcd_strobe();
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
    		LCD_DATA_WRITE |= (c & (0x0F)) << LCD_DATA_OFFSET;
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
}
//******************************************************************
// send data to the LCD
//******************************************************************
void lcd_data(unsigned char c){
        
	LCD_WAIT; // may check LCD busy flag, or just delay a little, depending on lcd.h

	LCD_DATA_WRITE &= ~OUTPUT_DATA;       
	LCD_RS(1);
	if (LCD_MODE == FOURBIT_MODE)
	{
    		LCD_DATA_WRITE &= ~OUTPUT_DATA;
                LCD_DATA_WRITE |= ((c >> 4) & 0x0F) << LCD_DATA_OFFSET;  
		lcd_strobe();		
                LCD_DATA_WRITE &= (0xF0 << LCD_DATA_OFFSET) | (0xF0 >> 8 - LCD_DATA_OFFSET);
                LCD_DATA_WRITE &= ~OUTPUT_DATA;
		LCD_DATA_WRITE |= (c & 0x0F) << LCD_DATA_OFFSET; 
		lcd_strobe();
	}
	else
	{
		LCD_DATA_WRITE = c;
		lcd_strobe();
	}
          
	LCD_RS(0);   
}
//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){
	unsigned char i;
        for(i=cnt ; i>0 ; i--) asm("noP"); // tha command asm("nop") takes raphly 1usec
	
}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(unsigned int cnt){
	unsigned char i;
        for(i=cnt ; i>0 ; i--) DelayUs(1000); // tha command asm("nop") takes raphly 1usec
	
}
//******************************************************************
// lcd strobe functions
//******************************************************************
void lcd_strobe(){
  LCD_EN(1);
//  asm("NOP");
  asm("nop");
  LCD_EN(0);
}
//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[msec]
	volatile unsigned int i;
	for(i=t; i>0; i--);
}
//--------------------------------------------------------------------
//              Print Byte to 8-bit LEDs array
//--------------------------------------------------------------------
void print2LEDs(unsigned char ch){
    LEDsArrPort = ch;
}
//--------------------------------------------------------------------
//              Clear 8-bit LEDs array
//--------------------------------------------------------------------
void clrLEDs(void){
    LEDsArrPort = 0x000;
}
//---------------------------------------------------------------------
//            key pad press
//---------------------------------------------------------------------
char key_pad_press(){
    char kb = 'r';

    KEY_PAD_OUT = 0x0e;
    if((KEY_PAD_IN & 0x10) == 0)  kb = 'p';
    else if((KEY_PAD_IN & 0x20) == 0)  kb = 'o';
    else if((KEY_PAD_IN & 0x40) == 0)  kb = 'n';
    else if((KEY_PAD_IN & 0x80) == 0)  kb = 'm';

    KEY_PAD_OUT = 0x0d;
    if((KEY_PAD_IN & 0x10) == 0)  kb = 'l';
    else if((KEY_PAD_IN & 0x20) == 0)  kb = 'k';
    else if((KEY_PAD_IN & 0x40) == 0)  kb = 'j';
    else if((KEY_PAD_IN & 0x80) == 0)  kb = 'i';

    KEY_PAD_OUT = 0x0b;
    if((KEY_PAD_IN & 0x10) == 0)  kb = 'h';
    else if((KEY_PAD_IN & 0x20) == 0)  kb = 'g';
    else if((KEY_PAD_IN & 0x40) == 0)  kb = 'f';
    else if((KEY_PAD_IN & 0x80) == 0)  kb = 'e';

    KEY_PAD_OUT = 0x07;
    if((KEY_PAD_IN & 0x10) == 0)  kb = 'd';
    else if((KEY_PAD_IN & 0x20) == 0)  kb = 'c';
    else if((KEY_PAD_IN & 0x40) == 0)  kb = 'b';
    else if((KEY_PAD_IN & 0x80) == 0)  kb = 'a';

    KEY_PAD_OUT = 0x00;
//    KEY_PAD_IN = 0xF0;
//    if (kb != 'r')
//        lcd_data(kb);
    return kb;
}
//---------------------------------------------------------------------
//            setSourceDestDMA0
//---------------------------------------------------------------------
void setSourceDestDMA0(int source, int dest){
   DMA0SA = source;
   DMA0DA = dest;
}
//---------------------------------------------------------------------
//            setSourceDestDMA2
//---------------------------------------------------------------------
void setSourceDestDMA2(int source, int dest){
   DMA2SA = source;
   DMA2DA = dest;
}
//---------------------------------------------------------------------
//            setSourceDestDMA1
//---------------------------------------------------------------------
void setSourceDestDMA1(int source){
   DMA1SA = source;
}
//---------------------------------------------------------------------
//            trigerDMA0
//---------------------------------------------------------------------
void trigerDMA0(){
    DMA0CTL |= DMAREQ + DMAEN;
}
//---------------------------------------------------------------------
//            trigerDMA1
//---------------------------------------------------------------------
void enableDMA1(){
    DMA1CTL |= DMAEN;
}
//---------------------------------------------------------------------
//            trigerDMA2
//---------------------------------------------------------------------
void trigerDMA2(){
    DMA2CTL |= DMAREQ + DMAEN;
}

//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
void enterLPM(unsigned char LPM_level){
	if (LPM_level == 0x00) 
	  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
        else if(LPM_level == 0x01) 
	  _BIS_SR(LPM1_bits);     /* Enter Low Power Mode 1 */
        else if(LPM_level == 0x02) 
	  _BIS_SR(LPM2_bits);     /* Enter Low Power Mode 2 */
	else if(LPM_level == 0x03) 
	  _BIS_SR(LPM3_bits);     /* Enter Low Power Mode 3 */
        else if(LPM_level == 0x04) 
	  _BIS_SR(LPM4_bits);     /* Enter Low Power Mode 4 */
}
//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts(){
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            keypad pressed
//---------------------------------------------------------------------
int keypadPressed(){
  if((KEY_PAD_IN & 0x80)&&(KEY_PAD_IN & 0x40)&&(KEY_PAD_IN & 0x20)&&(KEY_PAD_IN & 0x10)){
      return 0;
  }
  return 1;
}
//******************************************************************
// write a string of chars to the LCD
//******************************************************************
void lcd_puts(const char * s){
  
	while(*s)
		lcd_data(*s++);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}
//---------------------------------------------------------------------
//            Enable TimerA0
//---------------------------------------------------------------------
void enable_TimerB(){
  TBCTL |= MC_1 +TACLR;
}
//---------------------------------------------------------------------
//            Disable TimerA0
//---------------------------------------------------------------------
void disable_TimerB(){
  TBCTL &= ~(MC_1);
}
//---------------------------------------------------------------------
//            Enable TimerB ccie
//---------------------------------------------------------------------
void enable_TimerBCCIE(){
    TB0CCTL0 |= CCIE;
}
//---------------------------------------------------------------------
//            Disable TimerB ccie
//---------------------------------------------------------------------
void disable_TimerBCCIE(){
    TB0CCTL0 &= ~(CCIE);
}
//---------------------------------------------------------------------
//            ENABLE KEYPAD INTERUPTS
//---------------------------------------------------------------------
void enable_keypad(){
    KeyPadArrIntEn       |= 0x02;
}
//---------------------------------------------------------------------
//            disable KEYPAD INTERUPTS
//---------------------------------------------------------------------
void disable_keypad(){
    KeyPadArrIntEn       &= ~0x02;
}
/*
//---------------------------------------------------------------------
//            Enable TimerA1
//---------------------------------------------------------------------
void enable_TimerA1(){
  TA1CTL |= MC_2 + TACLR;
}
//---------------------------------------------------------------------
//            Enable TimerA1Q2
//---------------------------------------------------------------------
void enable_TimerA1Q2(){
  TA1CTL |= MC_1 + TACLR;
  TA1CCTL1 = OUTMOD_7;
}
//---------------------------------------------------------------------
//            Disable TimerA1
//---------------------------------------------------------------------
void disable_TimerA1(){
  TA1CTL &= ~MC_3;
  TA1CCTL1 = OUTMOD_0;
}
//---------------------------------------------------------------------
//            Enable TimerA1CCTL2
//---------------------------------------------------------------------
void enable_TA1CCTL2(){
  TA1CCTL2 |= CCIE;
}
//---------------------------------------------------------------------
//            Enable ADC10
//---------------------------------------------------------------------
void enable_ADC10(){
  ADC10CTL0 |= ENC + ADC10SC;             // Start sampling
}
//---------------------------------------------------------------------
//            Disable ADC10
//---------------------------------------------------------------------
void disable_ADC10(){
  
}

//---------------------------------------------------------------------
//            write freq template
//---------------------------------------------------------------------
void writeFreqLCD(){
  lcd_clear();
  lcd_home();
  const char fin[] = "fin=";
  const char hz[] = "Hz";
  lcd_puts(fin);
  lcd_cursor_right();
  lcd_cursor_right();
  lcd_cursor_right();
  lcd_cursor_right();
  lcd_cursor_right();
  lcd_puts(hz);
}
*/
//*********************************************************************
//            TimerB Interrupt Service Rotine
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMERB0_VECTOR//TIMERB0_VECTOR||TIMER0_B0_VECTOR
__interrupt void Timer_B (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer_B (void)
#else
#error Compiler not supported!
#endif
{
  LPM0_EXIT;
}
/*
//*********************************************************************
//            TimerA1 Interrupt Service Rotine
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) Timer1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(TA1IV){
  case 0x2:
    break;
  case 0x4:
    if(cycle == 0){
      cycle = 1;
      start = TA1CCR2;
    }
    else{
      cycle = 0;
      end = TA1CCR2;
      TA1CCTL2 &= ~CCIE;
      __bic_SR_register_on_exit(LPM0_bits + GIE);
    }  
    break;
  case 0xa:
    break;
  default:
    break;
  }
  
}



//*********************************************************************
//            ADC10 Interrupt Service Rotine
//*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

*/
//*********************************************************************
//            Port1 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT1_VECTOR
  __interrupt void PBs_handler(void){
      
	delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
	if(PBsArrIntPend & PB0){
	  state = state1;
	  PBsArrIntPend &= ~PB0;
        }
        else if(PBsArrIntPend & PB1){
	  state = state2;
	  PBsArrIntPend &= ~PB1; 
        }
	else if(PBsArrIntPend & PB2){ 
	  state = state3;
	  PBsArrIntPend &= ~PB2;
        }
        else if(PBsArrIntPend & PB3){    //for real time
	  state = state4;
	  PBsArrIntPend &= ~PB3;
       }

//---------------------------------------------------------------------
//            Exit from a given LPM 
//---------------------------------------------------------------------	
        switch(lpm_mode){
		case mode0:
		 LPM0_EXIT; // must be called from ISR only
		 break;
		 
		case mode1:
		 LPM1_EXIT; // must be called from ISR only
		 break;
		 
		case mode2:
		 LPM2_EXIT; // must be called from ISR only
		 break;
                 
                case mode3:
		 LPM3_EXIT; // must be called from ISR only
		 break;
                 
                case mode4:
		 LPM4_EXIT; // must be called from ISR only
		 break;
	}
        
}
//*********************************************************************
//            Port2 Interrupt Service Rotine
//*********************************************************************
#pragma vector=PORT2_VECTOR
  __interrupt void key_pad_handler(void){
      
	delay(debounceValKP);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
	
        if(KeyPadArrIntPend & KEY_PAD_PRESS){    //for real time
            input = key_pad_press();
            KeyPadArrIntPend &= ~KEY_PAD_PRESS;
       }

//---------------------------------------------------------------------
//            Exit from a given LPM 
//---------------------------------------------------------------------	
        switch(lpm_mode){
		case mode0:
		 LPM0_EXIT; // must be called from ISR only
		 break;
		 
		case mode1:
		 LPM1_EXIT; // must be called from ISR only
		 break;
		 
		case mode2:
		 LPM2_EXIT; // must be called from ISR only
		 break;
                 
                case mode3:
		 LPM3_EXIT; // must be called from ISR only
		 break;
                 
                case mode4:
		 LPM4_EXIT; // must be called from ISR only
		 break;
	}
        
}
 
  //*********************************************************************
  //            DMA interrupt service routine
  //*********************************************************************
  #pragma vector = DMA_VECTOR
  __interrupt void DMA_ISR(void)
  {
   DMA0CTL &= ~DMAIFG;
   DMA1CTL &= ~DMAIFG;
   DMA2CTL &= ~DMAIFG;
   __bic_SR_register_on_exit(LPM0_bits);       // Exit LPMx, interrupts enabled
  }

  //*********************************************************************
   //            state 2 - Data base and dma_strMirror
   //*********************************************************************

  char data_base[11][64] = {
      "An apple a day keeps the doctor away",
      "Give someone the benefit of the doubt",
      "Time flies when you're having fun",
      "He who pays the piper calls the tune",
      "The pen is mightier than the sword",
      "The pot calling the kettle black",
      "Wrap your head around something",
      "We'll cross that bridge when we come to it",
      "A bird in the hand is worth two in the bush",
      "Do unto others as you would have them do unto you",
      "You can catch more flies with honey than you can with vinegar",
      "eod" // data bade delimiter string
  };

