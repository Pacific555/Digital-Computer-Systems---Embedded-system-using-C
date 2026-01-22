#include  "../header/halGPIO.h"     // private library - HAL layer
#include  "../header/LCD.h"
#include "stdio.h"

unsigned int val0=0 , val1=0, overflowes = 0;
int flag = 0;  // first rising or second
int res;
int resetState2 =0;
//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
	GPIOconfig();
	TIMERconfig();
	ADCconfig();
	lcd_init();
	lcd_clear();

}

void sleep_gie(){
    __bis_SR_register(LPM0_bits + GIE);
}
//--------------------------------------------------------------------
//             Timers functions
//--------------------------------------------------------------------
void start_timer_state2(void){
    TIMER_ST2_CTL |=  MC_3;
}

void stop_timer_state2(void){
    TIMER_ST2_CTL =TIMER_ST2_RESET;
}

void start_timer_state1(void){
    TIMER_ST1_CTL |= TIMER_ST1_START ;

}

void stop_timer_state1(void){
    TIMER_ST1_CTL = TIMER_ST1_STOP ;
}


void  start_timer_st3(){
    TIMER_ST_3_CCTL1  = OUTMOD_7;
    TIMER_ST3_CTL |= MC_1; // START UP MODE

}

void stop_timer_st3(){
    TIMER_ST3_CTL = TBSSEL_2 + MC_0 + TBCLR;
    TIMER_ST_3_CCTL1  = 0;
}

void stop_timer_st4(){
    TIMER_ST2_CTL =  TIMER_ST2_RESET;
}

void start_timer_state4(void){
    TIMER_ST2_CTL |=  MC_3;
}


//--------------------------------------------------------------------
//             ADC functions
//--------------------------------------------------------------------
void enable_ADC_Interupts(){
    ADC_IE |= 0x01;
    ADCCTL0 |= ADC_ON_BIT;
    ADCCTL0 |= MSC ;
}

void disable_ADC_Interupts(){
    ADCCTL0 &= ~ADC_EN_BIT ;
}

void start_sampling(){
    ADCCTL0 |= ADC_EN_BIT+ADC_SC_BIT;
}

int return_res(){
    return res;
}

void set_pwm_t(int pwm_T, int duty_T){
    TIMER_ST_3_CCR0 = pwm_T;
    TIMER_ST_3_CCR1 = duty_T;
}

//---------------------------------------------------------------------
//           return state 1 values
//---------------------------------------------------------------------
void return_values_state1(int * rise1,int * rise2,int * overflow_num){
    *rise1 = val0;
    *rise2 = val1;
    *overflow_num = overflowes;
}

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
  _BIS_SR(GIE);
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts(){
  _BIC_SR(GIE);
}


//----------------------- resrt state 2-------------------------
void reset_timer_on(){
    resetState2 =1;
}

void reset_timer_of(){
    resetState2 =0;
}

int return_reset_timer(){
    return resetState2;
}


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





//*********************************************************************
//            Port2 Interrupt Service Rotine
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
      if(state == state2)  {
          resetState2 =1;
      }
      else {
      state = state2;
      }
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
 //            TIMER Interrupt Service Rotine
 //*********************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER_ST2_VEC
__interrupt void Timer_handler (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER_ST2_VEC))) Timer_handler (void)
#else
#error Compiler not supported!
#endif
{
    TIMER_ST2_CTL =TIMER_ST2_RESET;
    LPM0_EXIT; // must be called from ISR only

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER_ST1_VEC
__interrupt void TIMER1_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER_ST1_VEC))) TIMER1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(VECTOR_TIMER_ST1, 0x0A))
  {
      case  VECTOR_TIMER_ST1_CCR1:                   // Vector  2:  TACCR1 CCIFG

        break;
      case VECTOR_TIMER_ST1_CCR2:                    // Vector  4:  TACCR2 CCIFG
          if ( CCI)                 // Capture Input Pin Status
                  {
                      if(flag){
                          val1 = TIMER_ST1_CCR2;
                          flag ^= 1;
                         TIMER_ST1_CCTL2 &= ~CCIE;
                         LPM0_EXIT;

                      }

                      else
                      {
                          val0 = TIMER_ST1_CCR2;
                          flag ^= 1;
                      }

                  }
          break;
      case VECTOR_TIMER_ST1_IFG:
          //overflowes ++ ; break;              // Vector 10:  TAIFG
      default:  break;
  }
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    ADCCTL0 &= ~ADC_EN_BIT ;
    res = ADC_SAMPLE_REG;
    LPM0_EXIT;
}












