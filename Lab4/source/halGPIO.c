#include  "../header/halGPIO.h"     // private library - HAL layer
#include  "../header/LCD.h"
int X = 500;
char str[6] = {0};
unsigned int res;
char string[16] = {0};
//--------------------------------------------------------------------
//             System Configuration  
//--------------------------------------------------------------------
void sysConfig(void){ 
    init_uart();
	GPIOconfig();
	lcd_init();
	lcd_clear();
	base_timer_config();
	PWM_for_bazzer_config();
	ADC_config();
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
	    __bis_SR_register(LPM0_bits + GIE);
	//  _BIS_SR(LPM0_bits);     /* Enter Low Power Mode 0 */
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
        for(i=cnt ; i>0 ; i--) asm("Nop"); // tha command asm("nop") takes raphly 1usec

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
  asm("nop");
  LCD_EN(0);
}

void start_timer()
{
    TA0CTL |=  MC_1;
    TA0CCTL0  = CCIE;
}

void stop_timer()
{
    TA0CTL = TASSEL_2 + MC_0;
    TA0CCTL0  &= ~CCIE;
}
//////////////////////////////////////////////////////////////////////////

//#if defined(_TI_COMPILER_VERSION) || defined(IAR_SYSTEMS_ICC_)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_handler (void)
//#elif defined(_GNUC_)
//void _attribute_((interrupt(TIMER0_A0_VECTOR))) Timer_handler (void)
//#else
//#error Compiler not supported!
//#endif
{
    static int couter = 0;
    if(couter >= X ){
        couter = 0;
        LPM0_EXIT;
    }
    else
        couter++;

     // must be called from ISR only
}


int get_x(){
    return X;
}

int get_num(){
    int tmp;
    sscanf(str, "%d", &tmp);
    return tmp;
}

void setX(int num){
    X = num;
}

void define_RGB_color(char color){
    P2OUT |= color;
    P2OUT &= color;
}

void sleep_gie(){
    __bis_SR_register(LPM0_bits + GIE);
}
 

/////PWM FUNCTIONS///
void set_pwm_t(int pwm_T){
    TA1CCR0 = pwm_T;
    TA1CCR2 = pwm_T >> 1;
}

void  start_timer_st3(){
    TA1CCTL2  = OUTMOD_7;
    TA1CTL |= MC_1; // START UP MODE

}

void stop_timer_st3(){
    TA1CTL = TASSEL_2 + MC_0 + TACLR;
    TA1CCTL2  = 0;
}
////////////////////

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  static int isData = 0;
  static int i = 0;
  while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  if(isData == 0){
   switch(UCA0RXBUF){
   case '1':
       state = state1;
       LPM0_EXIT;
   break;
   case '2':
       state = state2;
       LPM0_EXIT;
   break;
   case '3':
       state = state3;
       LPM0_EXIT;
   break;
   case '4':
       state = state4;
       isData = 1;
       LPM0_EXIT;
   break;
   case '5':
       state = state5;
       LPM0_EXIT;
   break;
   case '6':
       state = state6;
       LPM0_EXIT;
   break;
   case '7':
       state = state7;
       LPM0_EXIT;
   break;
   case '8':
       state = state8;
       LPM0_EXIT;
   break;
   case '9':
        state = state0;
        LPM0_EXIT;
    break;
   }

  }
  else{
      if(UCA0RXBUF=='\n'){
          int j;
          state = state4;
          isData = 0;
          for(j = i; j<6;j++)str[j]=' ';
          LPM0_EXIT;
          i = 0;
      }else{
          str[i] = UCA0RXBUF;
          i++;
      }

  }

}




/////ADC////////////
void enable_ADC_Interupts(){
    ADC10CTL0 |= ADC10ON;
    ADC10CTL0 |= MSC ;
    ADC10CTL0 |=ADC10IE;
}

void disable_ADC_Interupts(){
    ADC10CTL0 &= ~ENC ;
   // ADC10CTL0 &= ~ADC10IE ;
}

void start_sampling(){
    ADC10CTL0 |= ENC + ADC10SC;
    sleep_gie();
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    ADC10CTL0 &= ~ENC ;
    res = ADC10MEM;
   // ADC10CTL0 &= ~ADC10IE;
    ADC10CTL0 &= ~ADC10IFG;
    LPM0_EXIT;
}

unsigned int get_res()
{
    return res;
}
///// ADC10ON + ADC10IE  ENC + ADC10SC
////

void enable_button(){
    P2IE |= 0x08;
}

void disable_button(){
    P2IE &=~ 0x08;
}

void send_str(char str[16] ){
    int j =  0;
    for (j = 0;j<16 ;j++)
        string[j] = str[j];
    IE2 |= UCA0TXIE;
   // char * temp = str;
  //  while(*temp){
  //      UCA0TXBUF = *temp;
   //     temp ++;
  //  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCI0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    static int i = 0;
  UCA0TXBUF = string[i++];                 // TX next character

  if (i == 16)  {
      IE2 &= ~UCA0TXIE;                       // Disable USCI_A0 TX interrupt
          i=0;
  }

}

//// ---------------buttons---------------------------
#pragma vector=PORT2_VECTOR
  __interrupt void PBs_handler(void){

      delay(debounceVal);
//---------------------------------------------------------------------
//            selector of transition between states
//---------------------------------------------------------------------
    if(P2IFG & PB1){
        P2IFG &= ~PB1;
        }
    LPM0_EXIT;
  }
