#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include  "../header/LCD.h"
#include "stdio.h"
#define ASCII_OFFSET 48
#define maxTBR  0xFFFF
#define subFreq  1048576


enum FSMstate state;
enum SYSmode lpm_mode;

//******************************************************************
// write a string of chars to the LCD
//******************************************************************
void lcd_puts(const char * s){

    while(*s)
        lcd_data(*s++);
}

//******************************************************************
// state 1
//******************************************************************


 void freqCount(){
     char firstTime = 0;
     unsigned int num_of_samples = 0;
     unsigned int  last_freq;
     state1_timer_init();
     unsigned int rise1,rise2,overflow_num,sigFreq;
     unsigned int n =0;// N is the number of SubMaster clock cycles
     stop_timer_st3();
     WDTCTL = WDTPW + WDTHOLD;
     lcd_clear();
     start_timer_state1();
     while(state == state1){
         //TIMER_ST1_CCTL2 |= CCIFG;
         disable_interrupts();
         TIMER_ST1_CCTL2 |= CCIE;
         sleep_gie();
         return_values_state1(&rise1,&rise2,&overflow_num);
         n =  rise2+(maxTBR * overflow_num)  - rise1;
         sigFreq = subFreq / n;
         if(firstTime ==0) {
             unsigned int i;
             firstTime  = 1;
             last_freq = sigFreq;
             lcd_home();
             lcd_puts("fin = ");
             for(i=0;i<5;i++) lcd_cursor_right();
                 lcd_puts("Hz");
         }
         if(!(last_freq >= sigFreq * 0.98 && last_freq <= sigFreq *1.02 )&&sigFreq!=-1){
             num_of_samples++;
         if(num_of_samples>=50){
             last_freq = sigFreq;
             printFreq(sigFreq);
             num_of_samples = 0;
         }
      }
  enable_interrupts();
 }
     stop_timer_state1();
}



 void printFreq(unsigned int freq){
     char str_freq[6] = "     ";
     char tmp_str[6] = {'\0'};
     moveToFreq();
    sprintf(tmp_str, "%d", freq);
     int i = 0;
     while(*(tmp_str+i))
     {
         str_freq[i] = tmp_str[i];
         i++;
      //   tmp_str ++;
     }
     lcd_puts(str_freq);
 }

     void moveToFreq(){
         int i;
         lcd_home();
         for( i=0;i<5;i++) lcd_cursor_right();
     }



//******************************************************************
// state 2
//******************************************************************



void countMinute(){
    char minutes = 0, seconds_units = 0,  seconds_tens = 0, counter = 0; // initializing count
    reset_timer_of();
    lcd_clear();
    WDTCTL = WDTPW + WDTHOLD;
    while(state == state2 && counter < 121&& return_reset_timer()==0) {
        PrintTime(&minutes, &seconds_units, &seconds_tens);
        UpdateDigits(&minutes, &seconds_units, &seconds_tens, &counter);
        start_timer_state2();
        sleep_gie();
    }

    counter = 0;
}

void PrintTime(char * minutes, char * seconds_units, char * seconds_tens)
{
    lcd_home();
    lcd_putchar('0');
    lcd_putchar(*minutes + ASCII_OFFSET);
    lcd_putchar(':');
    lcd_putchar(*seconds_tens + ASCII_OFFSET);
    lcd_putchar(*seconds_units + ASCII_OFFSET);
}

void UpdateDigits(char * minutes, char * seconds_units, char * seconds_tens, char* counter){
    if (*counter < 59){
        if(*seconds_units == 9){
               ( *seconds_tens)++;
                *seconds_units = 0;
        }
        else
           (*seconds_units)++;
    }
    else if ( *counter == 59)
    {
        *seconds_tens = 0;
        *seconds_units = 0;
        *minutes = 1;
    }
    else
    {
        if(*minutes == 1){
            *minutes = 0;
            *seconds_tens = 5;
            *seconds_units = 9;
        }
        else if(*seconds_units == 0){
                   (*seconds_tens)--;
                   *seconds_units = 9;
           }
        else {(*seconds_units)--;}
    }
    (*counter)++;
}



//******************************************************************
// state 3
//******************************************************************

void tone_gen(){
    unsigned int  m_qformat = 0x177,  f_out, T;
    WDTCTL = WDTPW + WDTHOLD;
    lcd_clear();
    start_timer_st3();
    unsigned  int sample_vec;
    while(state == state3){
        enable_ADC_Interupts();
        start_sampling() ;
        sleep_gie();
        disable_ADC_Interupts();
        sample_vec = return_res();
        float  N_out = -0.1536 *sample_vec + 1048;
        //T = subFreq / N_out;
        T = (int)N_out;
        set_pwm_t(T, (int)T/2);
       enable_interrupts();
    }
    stop_timer_st3();
    disable_ADC_Interupts();
}



//******************************************************************
// state 4
//******************************************************************
void print_names(char * f_name1, char * f_name2, char * l_name1, char*l_name2){
    int fl = 1;
    int index = 0;
    char * tmp;
    lcd_clear();
    while(state == state4){
        fl ^=1;
        start_timer_state2();
        sleep_gie();
        if(fl==0)
            continue;
        switch(index){
              case 0:
                 tmp =f_name1;
                 index++;
                 break;
              case 1:
               tmp =f_name2;
               index++;
               break;
              case 2:
                tmp =l_name1;
                index++;
                break;
              case 3:
                tmp =l_name2;
                index = 0;
                break;
        }
        if(index%2)
            lcd_home();

        else
            lcd_new_line;

        lcd_puts(tmp);

    }
}

 
  

