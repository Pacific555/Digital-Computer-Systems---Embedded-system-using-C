#include <header/api.h>    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
#include  <string.h>
#include  <math.h>


char input;
char idiom_recorder[32];
char str_swap[158];
char str[158] = "Google Colaboratory is a free Jupyter notebook environment that runs on Google’s cloud servers, letting the user leverage backend hardware like GPUs and TPUs";
int count_state1;
char clcFreq[6];
extern char data_matrix_in[10][36];
extern char data_matrix_out[10][36];
extern char printLEds[8];
extern int ledsValue;

//--------------------------------------------------------------------
//            timers setup for 1,0.5 sec
//--------------------------------------------------------------------
void timerBOnfor1sec(){
  enable_TimerB();
  enterLPM(0);
  enable_TimerB();
  enterLPM(0);
}
void timerBOnforhalfsec(){
  enable_TimerB();
  enterLPM(0);
}
//-----------------------------------------------------------------------------------------------------------------
//                                  state1 - record a msg
//-----------------------------------------------------------------------------------------------------------------

void idiom_record(){
    enable_TimerBCCIE();
    lcd_clear();
    lcd_home();
    lcd_puts("insert input :");
    count_state1 = -1;
    enable_keypad();

    char indexToKey[16][4] = {"1G","2H","3I","CJ","4K","5L","6M","DN","7O","8P","9Q","ER","AS","0T","BWX","FYZ"};

    unsigned int index;
    enterLPM(0);
    while(keypadPressed() == 0 && (state == state1))
        enterLPM(0);
    lcd_clear();
    lcd_home();

    while(count_state1<32 && (state == state1)){
        if(input != 'r'){
            count_state1 +=1;
            if(count_state1 == 16){
                lcd_home();
                lcd_new_line;
            }

            index = input-'a';
            unsigned int first = 1;
            unsigned int pos = 0;
            while(index == input-'a'&&(first || keypadPressed())){
                if(!first)
                    lcd_cursor_left();
                first = 0;
                idiom_recorder[count_state1] = indexToKey[index][pos];

                lcd_data(indexToKey[index][pos]);
                timerBOnfor1sec();      //sleep for sec
                if(index<14){          //14 of 2 char words in indexToKey, and 2 words of 3 chars
                  if(pos==1){
                      pos =0;
                  }else pos=1;
                }else{
                    if(pos==0){
                        pos = 1;
                    }else if(pos == 1){
                        pos = 2;
                    }else pos = 0;
                }

            }
            disable_TimerB();
            input = 'r';

        }
        enterLPM(0);
    }
    disable_keypad();


}
//-----------------------------------------------------------------------------------------------------------------
//                                  state2 - mirror a line in the matrix
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//                                  state2 - mirror a line in the matrix with scrolling
//-----------------------------------------------------------------------------------------------------------------
void mirror_string() {
    char strout[64] = {0};   // Output buffer for the mirrored string
    unsigned int index;
    unsigned int len;
    unsigned int i = 0;    // Index for the 32-char window

    //Prompt the user for the line number
    enable_keypad();
    lcd_clear();
    lcd_home();
    lcd_puts("insert line:");
    enterLPM(0);

    // Read valid index (0–10)
    while((index = input-'a') > 11){
        lcd_clear();
        lcd_home();
        lcd_puts("insert line:");
        enterLPM(0);
    }
    disable_keypad();

    //Mirror the string at that index
    len = strlen(data_base[index]);
    DMA0SZ = len;
    DMA0SA = (unsigned int)&data_base[index][len - 1];
    DMA0DA = (unsigned int)&strout[0];
    DMA0CTL = DMADT_1 | DMASBDB | DMAEN | DMADSTINCR_3 | DMASRCINCR_2;
    DMA0CTL |= DMAREQ;
    trigerDMA0();

    strout[len] = '\0';

    //Display the first segment (32 chars max)
    enable_keypad();
    lcd_clear();
    lcd_home();
    printLcdUP(strout);
    printLcdDOWN(strout);

    //Scroll through the mirrored string using the keypad
    enterLPM(0);      // Wait for keypad input
    lcd_clear();

    // Determine direction of scroll
    if(input - 'a' > 7){  // Next
        if(i + 32 >= len){   // If at end, wrap to start
            i = 0;
        } else {
            i += 32;
        }
    } else {                // Previous
        if(i == 0){         // If at start, wrap to end window
            i = (len <= 32) ? 0 : (len - 32);
        } else {
            i -= 32;
        }
    }

    // Show the new 32-char window (or whatever’s left)
    printLcdUP(strout + i);
    printLcdDOWN(strout + i);
    disable_keypad();
    enterLPM(0);      // Wait for keypad input

}

//-----------------------------------------------------------------------------------------------------------------
//                                  state2old - swap line of matrix
//-----------------------------------------------------------------------------------------------------------------
//
//void lineSwap(){
//    unsigned int index1;
//    unsigned int index2;
//    enable_keypad();
//    lcd_clear();
//    lcd_home();
//    lcd_puts("insert line:");
//    enterLPM(0);
//
//    while((index1 = input-'a')>9){
//        lcd_clear();
//        lcd_home();
//        lcd_puts("insert line:");
//        enterLPM(0);
//    }
//    lcd_clear();
//    lcd_home();
//    lcd_puts("insert line 2:");
//    enterLPM(0);
//
//    while((index2 = input-'a')>9){
//        lcd_clear();
//        lcd_home();
//        lcd_puts("insert line 2:");
//        enterLPM(0);
//    }
//    disable_keypad();
//    lcd_clear();
//    lcd_home();
//    unsigned int i;
//    setSourceDestDMA0(data_matrix_in[0]+1, data_matrix_out[0]);
//    trigerDMA0();
//    for(i=0;i<10;i++){
////        if(index1 == 0)
//        if(i==index1){
//            setSourceDestDMA0(data_matrix_in[index2], data_matrix_out[index1]);
//        }
//        else if(i==index2){
//            setSourceDestDMA0(data_matrix_in[index1], data_matrix_out[index2]);
//        }
//        else{
//            setSourceDestDMA0(data_matrix_in[i], data_matrix_out[i]);
//        }
//        trigerDMA0();
//    }
//    i = 0;
//    enable_keypad();
//    lcd_clear();
//    lcd_home();
//    printLcdUP(data_matrix_out[i]);
//    printLcdDOWN(data_matrix_out[i]);
//    while(state == state2){
//        enterLPM(0);
//        lcd_clear();
//        if(input - 'a' > 7){
//            if (i == 9){
//                i = 0;
//                printLcdUP(data_matrix_out[i]);
//                printLcdDOWN(data_matrix_out[i]);
//            }
//            else{
//                i++;
//                printLcdUP(data_matrix_out[i]);
//                printLcdDOWN(data_matrix_out[i]);
//            }
//        }
//        else{
//            if (i == 0){
//                i = 9;
//                printLcdUP(data_matrix_out[i]);
//                printLcdDOWN(data_matrix_out[i]);
//            }
//            else{
//                i--;
//                printLcdUP(data_matrix_out[i]);
//                printLcdDOWN(data_matrix_out[i]);
//            }
//        }
//    }
//    disable_keypad();
//    lcd_clear();
//}
void printLcdUP(char *toPrint){
    lcd_home();
    int i;
    for(i = 0; i<16; i++){
        if(toPrint[i] == 0)
            continue;
        lcd_data(toPrint[i]);
    }
}
void printLcdDOWN(char *toPrint){
    lcd_new_line;
    int i;
    for(i = 0; i<16; i++){
        if(toPrint[i+16] == 0)
            continue;
        lcd_data(toPrint[i+16]);
    }
}
//-----------------------------------------------------------------------------------------------------------------
//                                  state3 - print array on leds
//-----------------------------------------------------------------------------------------------------------------
//int arr[]={128,224,32,126,83,44,253,113,160};
//void Print_Arr_on_Leds(){
//    disable_TimerBCCIE();
//    clrLEDs();
//    int count_q3 = 0;
//    enableDMA1();
//    while(count_q3 <= 8){
//        DMA1SA = (unsigned int)&arr[count_q3];
//        enableDMA1();
//        timerBOnforhalfsec();
//        count_q3++;
//        disable_TimerBCCIE();
//    }
//    clrLEDs();
//    disable_TimerB();
//    state = state0;
//}
int arr[9]={128,224,32,126,83,44,253,113,160};
void Print_Arr_on_Leds(){
    disable_TimerBCCIE();
    clrLEDs();
    int count_q3 = 0;

    while(count_q3 <= 9){
        DMA1SA = (unsigned int)&arr[count_q3];     // Set source address
        DMA1CTL &= ~DMAIFG;                        // Clear DMA interrupt flag
        DMA1CTL |= DMAEN + DMAREQ;                 // Enable + trigger DMA in same line
        timerBOnforhalfsec();                      // 0.5 sec delay for visible LED effect
        count_q3++;
    }

    clrLEDs();    // clear LEDs after final element
    disable_TimerB();
    state = state0;
}


//-----------------------------------------------------------------------------------------------------------------
//                                  state3 old - turn on leds cyclicly
////-----------------------------------------------------------------------------------------------------------------
//void Right2LeftLeds(){
//    disable_TimerBCCIE();
//    ledsValue = 0x01;
//    int count_q3 = 0;
//    setSourceDestDMA1(&ledsValue);
//    while(state==state3){
//        enableDMA1();
//        timerBOnforhalfsec();
//        ledsValue =ledsValue << 1;
//        if(count_q3 == 8){
//            count_q3 = -1;
//            ledsValue = 0x01;
//        }
//        count_q3++;
//    }
//    disable_TimerB();
//    clrLEDs();
//}
//-----------------------------------------------------------------------------------------------------------------
//                                  state4old  - swap string
//-----------------------------------------------------------------------------------------------------------------
//void Swap_String(){
//    lcd_home();
//    lcd_clear();
//    setSourceDestDMA2(str + 157,str_swap);
//    trigerDMA2();
//    printLcdUP(str_swap + 1);
//    printLcdDOWN(str_swap + 1);
//}
//
//-----------------------------------------------------------------------------------------------------------------
//                                  state4  - swap half string
//-----------------------------------------------------------------------------------------------------------------
void swap_half_string() {
    char strout[64] = {0};   // Output buffer for the mirrored string
    unsigned int index;
    unsigned int len;
    unsigned int i = 0;    // Index for the 32-char window

    //Prompt the user for the line number
    enable_keypad();
    lcd_clear();
    lcd_home();
    lcd_puts("insert line:");
    enterLPM(0);

    // Read valid index (0–10)
    while((index = input-'a') > 11){
        lcd_clear();
        lcd_home();
        lcd_puts("insert line:");
        enterLPM(0);
    }
    disable_keypad();

    //swap the first half string at that index
    len = strlen(data_base[index]);
    int half = floor(len/2);
    DMA0SZ = len - half;
    DMA0SA = (unsigned int)&data_base[index][half];
    DMA0DA = (unsigned int)&strout[0];
    DMA0CTL = DMADT_1 | DMASBDB | DMAEN | DMADSTINCR_3 | DMASRCINCR_3;
    DMA0CTL |= DMAREQ;
    trigerDMA0();

    //swap the second half string at that index
    DMA0SZ = floor(len/2);
    DMA0SA = (unsigned int)&data_base[index][0];
    DMA0DA = (unsigned int)&strout[len - half];
    DMA0CTL = DMADT_1 | DMASBDB | DMAEN | DMADSTINCR_3 | DMASRCINCR_3;
    DMA0CTL |= DMAREQ;
    trigerDMA0();

    strout[len] = '\0';

    //Display the first segment (32 chars max)
    enable_keypad();
    lcd_clear();
    lcd_home();
    printLcdUP(strout);
    printLcdDOWN(strout);

    //Scroll through the mirrored string using the keypad
    enterLPM(0);      // Wait for keypad input
    lcd_clear();

    // Determine direction of scroll
    if(input - 'a' > 7){  // Next
        if(i + 32 >= len){   // If at end, wrap to start
            i = 0;
        } else {
            i += 32;
        }
    } else {                // Previous
        if(i == 0){         // If at start, wrap to end window
            i = (len <= 32) ? 0 : (len - 32);
        } else {
            i -= 32;
        }
    }

    // Show the new 32-char window (or whatever’s left)
    printLcdUP(strout + i);
    printLcdDOWN(strout + i);
    disable_keypad();
    enterLPM(0);      // Wait for keypad input

}




char* dma_strSwap(char** data, int idx, char* strout)
{

       int len = strlen(data_base[idx]);
       int half = floor(len/2);

       //swap the first half string at that index
       DMA0SZ = len - half;
       DMA0SA = (unsigned int)&data[idx][half];
       DMA0DA = (unsigned int)&strout[0];
       DMA0CTL = DMADT_1 | DMASBDB | DMAEN | DMADSTINCR_3 | DMASRCINCR_3;
       DMA0CTL |= DMAREQ;
       trigerDMA0();

       //swap the second half string at that index
       DMA0SZ = half;
       DMA0SA = (unsigned int)&data[idx][0];
       DMA0DA = (unsigned int)&strout[len - half];
       DMA0CTL = DMADT_1 | DMASBDB | DMAEN | DMADSTINCR_3 | DMASRCINCR_3;
       DMA0CTL |= DMAREQ;
       trigerDMA0();

       strout[len] = '\0';
    //   return strout;
}





















