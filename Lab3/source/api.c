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
void state1_func(char S[33]){
    char let,flag;
    unsigned int count=0;
    lcd_clear();
    KEYPAD_IRQ_IE |= KEYPAD_IRQ_INDEX;
    while(state == state1){
        sleep_gie();
        flag = return_flag();
        if(flag==0)start_timer();
        if(count < 32){
            let = return_char();
            if(let!='#'){
                if(count == 16)
                    lcd_new_line;
                if(flag!=0){
                    lcd_cursor_left();
                }else{
                    count++;
                }
                lcd_putchar(let);
                S[count-1] = let;
            }

        }
    }
    KEYPAD_IRQ_IE &= ~KEYPAD_IRQ_INDEX;
}






//******************************************************************
// state 2
//******************************************************************
void state2_func(unsigned int i,unsigned int j,  char src_matrix[][M] ,  char dst_matrix[][M] ){
    int global_count = 0 , k = 0 ;
    char line[16] = {0};
    char * current_st;
    KEYPAD_IRQ_IE |= KEYPAD_IRQ_INDEX;
    transferBlock(src_matrix[0], dst_matrix[0], N * M);
    transferBlock(src_matrix[i], dst_matrix[j],  M);
    transferBlock(src_matrix[j], dst_matrix[i],  M);

    PBsArrIntEn = 0x00;   /// disable PUSHBUTTONS interupts- Atomic state

    current_st =dst_matrix[k];

    while(*current_st && global_count < 16){
          line[global_count++] = *current_st  ;
          current_st++;
  }
  if(global_count < 16) current_st =dst_matrix[++k];
  global_count=0;
  while(k < N)
    {
        lcd_home();
        lcd_clear();
        lcd_puts(line);
        lcd_new_line; //go downward
        while(*current_st && global_count < 16){
                   line[global_count++] = *current_st;
                   current_st++;

        }

        line[global_count] = 0;
        lcd_puts(line);

        if(global_count < 16){
                 current_st =dst_matrix[++k];
        }
        global_count = 0;

        sleep_gie();
    }
  lcd_clear();
  PBsArrIntEn |= 0x0F;
  KEYPAD_IRQ_IE &= ~KEYPAD_IRQ_INDEX;
}


//******************************************************************
// state 3
//******************************************************************

void state3_func(int * arr){
        lcd_clear();
        DMA_start(arr);
        PBsArrIntEn = 0x00;
      __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0
         DMA_stop();
}


void state4_func(char * src, char * dst, int blk_sz)
{
    int i;
    lcd_clear();
    DMA_ST4_start(src+blk_sz-2, dst,  blk_sz);
    for(i=32;i>0;i--){
        if(i==16)lcd_new_line;
        lcd_data(*dst++);

    }


}




//******************************************************************
// state 4
//******************************************************************


 
  

