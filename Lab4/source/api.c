#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include "../header/LCD.h"
//enum FSMstate state;

//******************************************************************
// write a string of chars to the LCD
//******************************************************************
int count_state2 = 0;

void lcd_puts(const char * s){

    while(*s)
        lcd_data(*s++);
}

//-----------------------------------State 1--------------------------------------------------
void state1_func(void){
    int x = get_x();
    char binary_count = 0;
    while(state == state1){
        start_timer();
        sleep_gie();
        stop_timer();
        define_RGB_color(binary_count % 8);     // chang RGB color
        binary_count++;
    }
    define_RGB_color(0);
}
//-----------------------------------State 2--------------------------------------------------

void state2_func(void){
    int x = get_x();
    char str [32]={0};
        lcd_clear();
        while(state == state2){
            start_timer();
            sleep_gie();
            stop_timer();
           sprintf(str,"%d", count_state2);
           lcd_home();
            lcd_puts(str);
            count_state2++;
        }
}

//-----------------------------------State 3--------------------------------------------------
void state3_func(void){
   int freqs[7] = {1049, 839, 699, 599, 524, 466, 419};
   int x = get_x();
   char freq_index = 0;
   //lcd_clear();
   set_pwm_t(freqs[0]);
   start_timer_st3();
          while(state == state3){
              start_timer();
              sleep_gie();
              stop_timer();
              freq_index++;
              set_pwm_t(freqs[freq_index % 7]);
          }
   stop_timer_st3();
}

//-----------------------------------State 4--------------------------------------------------

void state4_func(){
    int num_x;
    __bis_SR_register(LPM0_bits + GIE);
    num_x = get_num();
    setX(num_x);
}

//-----------------------------------State 5--------------------------------------------------
void state5_func(){
        int j,counter = 0,fl=0;
        char str [6] = {0};
        char str_new[6] = {0};
        lcd_clear();
        enable_ADC_Interupts();
        unsigned  int sample_vec, Q_format, integer, frac;
        while(state == state5){
            start_sampling();
          //  sleep_gie();
            disable_ADC_Interupts();
            sample_vec = get_res();
            sample_vec *= 7;
            Q_format = (sample_vec >> 3); // Q8.8
            integer = get_int_from_Q(Q_format);
            frac =  get_frac_from_Q(Q_format);
            sprintf(str_new,"%d", integer);
            sprintf(str_new + 2,"%d", frac);
            str_new[1] = '.';

            if(fl==0){
                sprintf(str,"%d", integer);
                sprintf(str + 2,"%d", frac);
                str[1] = '.';
            }
            if(counter==300){
                lcd_home();
                for(j=0;j<5;j++){
                    if(str[j]!=str_new[j]){
                        lcd_putchar(str_new[j]);
                        str[j] = str_new[j];
                    }else lcd_putchar(str_new[j]);//lcd_cursor_right();
                }
                counter = 0;
                fl = 1;
            }else counter++;
        }
}

unsigned int get_frac_from_Q(unsigned int Q )
{
    unsigned int sum = 0, carry, j;

    for(j =0; j < 8; j ++)
    {
        carry = Q % 2;
        Q = Q >> 1;
        switch (j){
        case 7: sum += 500 * carry; break;
        case 6: sum += 250 * carry; break;
        case 5: sum += 125 * carry; break;
        case 4: sum += 62 * carry; break;
        case 3: sum += 31 * carry; break;
        }
    }

    return sum;
}
 
unsigned int get_int_from_Q(unsigned int Q )
{
    return Q >> 8;
}

//--------------------------State 6----------------------------------------
void state6_func(){
    lcd_clear();
    count_state2 =0;
}


///==========Real time---
void state7_func(){
    char str [16] = "I love my Negev";
 //   while(state == state7){
        enable_button();
        sleep_gie();
     //  if(state == state7)
         send_str(str);

  //  }
     disable_button();
}




