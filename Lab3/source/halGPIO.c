#include  "../header/halGPIO.h"     // private library - HAL layer
#include  "../header/LCD.h"
#include "stdio.h"

char let='#';
char flag = 0;
//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	TIMERconfig();
	lcd_init();
	lcd_clear();

}

void sleep_gie(){
    __bis_SR_register(LPM0_bits + GIE);
}
//--------------------------------------------------------------------
//             Timers functions
//--------------------------------------------------------------------

//---------------------------------------------------------------------
//            Polling based Delay function
//---------------------------------------------------------------------
void delay(unsigned int t){  // t[micro sec]
	volatile unsigned int i;
	for(i=t; i>0; i--);
}

void delayInMs(int t){ // t[m sec]
    volatile unsigned int i;
   for(i=10;i>0;i--) delay(t*10);
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
    __bis_SR_register(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
    __bic_SR_register(GIE);
}


//----------------------- resrt state 2-------------------------



//---------------------------------------------------------------------
//  LCD
//---------------------------------------------------------------------


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

    lcd_cmd(0x0F); //Display On, Cursor On, Cursor Blink
    lcd_cmd(0x01); //Display Clear
    lcd_cmd(0x6); //Entry Mode
    lcd_cmd(0x80); //Initialize DDRAM address to zero
    lcd_clear();
}
//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){

    unsigned char i;
        for(i=cnt ; i>0 ; i--) asm("nop"); // tha command asm("nop") takes raphly 1usec

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
  asm("NOP");
  asm("Nope");    // DIDNT COMPILE
  LCD_EN(0);
}


//******************************************************************
// return char state1
//******************************************************************

char return_char(){
    char tmp;
    tmp = let;
    let ='#';
    return tmp;
}

char return_flag(){
    char tmp;
    tmp = flag;
   flag++;
    return tmp;
}

//******************************************************************
//timer functions
//******************************************************************
void start_timer(){
    TACTL = TASSEL_2 + MC_2 +ID_3+TAIE;
}

void stop_timer(){
    TACTL = TASSEL_2 + MC_0 +ID_3+TACLR;
    TACTL &=~TAIE;
}
//******************************************************************
//DMA functions
//******************************************************************
void DMA_start(int *arr){
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    DMACTL0 = DMA0TSEL_2;                     // CCR2 trigger
    DMA0SA = (void (*)())arr;                  // Source block address
    DMA0DA = (void (*)())&P9OUT;                     // Destination single address
    DMA0SZ = 0x10;                            // Block size
    DMA0CTL = DMADT_4 + DMASRCINCR_3 + DMASBDB + DMAEN+DMAIE; // Rpt, inc src
    TBCTL = TBSSEL_2 + MC_2 +ID_2;                  // SMCLK/4, contmode, half a second
}

void DMA_stop(){
    DMA0CTL &= ~DMAEN + ~DMAIE;
    P9OUT &= ~0xFF;
    TBCTL &=MC_0;
}

void transferBlock(char * addr_src, char * adrr_dst, int blk_sz){
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    DMA1SA = (void (*)( ))addr_src;            // Start block address
    DMA1DA = (void (*)( ))adrr_dst;            // Destination block address
    DMA1SZ = blk_sz;                          // Block size
    DMA1CTL = DMADT_1 + DMASRCINCR_3 + DMADSTINCR_3 + DMASRCBYTE + DMADSTBYTE; // Rpt, inc
    DMA1CTL |= DMAEN;                         // Enable DMA0
    DMA1CTL |= DMAREQ;                        // triger DMA
}

void DMA_ST4_start(char * addr_src, char * adrr_dst, int blk_sz){
       WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
       DMA2SA = (void (*)( ))addr_src;            // Start block address
       DMA2DA = (void (*)( ))adrr_dst;            // Destination block address
       DMA2SZ = blk_sz;                          // Block size
       DMA2CTL = DMADT_1 + DMASRCINCR_2 + DMADSTINCR_3 + DMASRCBYTE + DMADSTBYTE; // Rpt, inc
       DMA2CTL |= DMAEN;                         // Enable DMA0
       DMA2CTL |= DMAREQ;                        // triger DMA
}



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
    else if(PBsArrIntPend & PB3){
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
    __interrupt void KEYPAD_REQ(void){
        if(state==state1){
           KEYPAD_OUT = 0X0F;
           KEYPAD_OUT = 0X0E;
           if((KEYPAD_IN & 0x10) == 0){
               switch(flag%3){
                 case 0:
                     let = 'F';
                    break;
                    case 1:
                       let = 'Y';
                    break;
                    case 2:
                       let = 'Z';
                    break;
                 }
           }
           else if((KEYPAD_IN & 0x20) == 0){
               switch(flag%3){
               case 0:
                      let = 'B';
                  break;
                  case 1:
                      let = 'W';
                  break;
                  case 2:
                     let = 'X';
                  break;
               }
           }
           else if((KEYPAD_IN & 0x40) == 0) (flag%2==0)?(let = '0'):(let = 'T');
           else if((KEYPAD_IN & 0x80) == 0) (flag%2==0)? (let = 'A'):(let = 'S');

       KEYPAD_OUT = 0X0F;
       delay(20);
       KEYPAD_OUT = 0X0D;
        if((KEYPAD_IN & 0x10) == 0)  (flag%2==0)?(let = 'E'):(let = 'R');
        else if((KEYPAD_IN & 0x20) == 0) (flag%2==0)?(let = '9'):(let ='Q');
        else if((KEYPAD_IN & 0x40) == 0) (flag%2==0)?(let = '8'):(let ='P');
        else if((KEYPAD_IN & 0x80) == 0)  (flag%2==0)?(let = '7'):(let = 'O');
        KEYPAD_OUT = 0X0F;
        delay(20);
        KEYPAD_OUT = 0X0B;
        if((KEYPAD_IN & 0x10) == 0)(flag%2==0)? (let = 'D'):(let = 'N');
        else if((KEYPAD_IN & 0x20) == 0) (flag%2==0)?(let='6'):(let = 'M');
        else if((KEYPAD_IN & 0x40) == 0) (flag%2==0)?(let ='5'):(let = 'L');
         else if((KEYPAD_IN & 0x80) == 0)  (flag%2==0)?(let ='4'):(let = 'K');
        KEYPAD_OUT = 0X0F;
        delay(20);
        KEYPAD_OUT = 0X07;
        if((KEYPAD_IN & 0x10) == 0) (flag%2==0)?(let = 'C'):(let = 'J');
        else if((KEYPAD_IN & 0x20) == 0) (flag%2==0)?(let = '3'):(let = 'I');
        else if((KEYPAD_IN & 0x40) == 0) (flag%2==0)?(let = '2'):(let = 'H');
        else if((KEYPAD_IN & 0x80) == 0) (flag%2==0)?(let = '1'):(let = 'G');
       }
      delay(debounceVal);
      KEYPAD_IRQ_IFG &= ~KEYPAD_IRQ_INDEX;
      KEYPAD_OUT  &= ~0x0F;

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
      //__bis_SR_register(GIE);

    }
  //---------------------------------------------------------------------
  //            selector of transition between states
  //---------------------------------------------------------------------

 //*********************************************************************
 //            TIMER Interrupt Service Rotine
 //*********************************************************************

/*
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER_B_VEC
__interrupt void TIMER_B_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER_B_VEC))) TIMER_B_ISR (void)
#else
#error Compiler not supported!
#endif
{

}
*/

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER_A_VEC
__interrupt void TIMER_A_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt())) TIMER_A_VEC (void)
#else
#error Compiler not supported!
#endif
{
flag = 0;
TACTL&=~ TAIFG;
TACTL = TASSEL_2 + MC_0 +ID_3+TACLR;
TACTL &=~TAIE;
}



#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DMA_VECTOR
  __interrupt void DMA0_handler(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DMA_VECTOR))) DMA0_handler (void)
#else
#error Compiler not supported!
#endif
  {
      switch(DMAIV){
      case 0x02:
        // if(LEDSOUT == 0x00)
          __bis_SR_register(GIE);
             PBsArrIntEn |= 0x0F;
       //  else
          //   disable_interrupts();
        //break;
      }

  }




