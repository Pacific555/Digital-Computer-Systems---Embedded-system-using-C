#include  "../header/api.h"    		// private library - API layer
#include  "../header/halGPIO.h"     // private library - HAL layer
//enum FSMstate state;
#define name_size 8
char value_led_shift = 1; //saving the value of the result of shifting



//--------------------------------------------------------------------
//            Print digits array to LEDs array with rate
//--------------------------------------------------------------------
void printDigitArr2LEDs(char id_arr[] , int size, unsigned int rate ){
    unsigned int i;
    for(i=0; i<size; i++){
        print2LEDs(id_arr[i]);
       // delayHalfSecond();
        delayInMs(rate);
       }
    clrLEDs();
}

//--------------------------------------------------------------------
//           shift the leds left
//--------------------------------------------------------------------
void ShiftNum2LEDs( unsigned int rate ){
   // char temp = value_led_shift;
    unsigned int i;
    print2LEDs(value_led_shift);
    delay(rate);
    for(i=0; i<14; i++){
        value_led_shift=shiftLEDs();
        delayInMs(rate);
       }
    clrLEDs();
   // value_led_shift = temp;
}


 void generatePwm(void){
     clearPWM(); /// Signal starts at zero


  while(state==state3){
         delay(delay_0);
         toggeleBit();
         delay(delay_1);
         toggeleBit();
     }
 }

 void PrintStr(char str[] , int delay_ms) {
     printDigitArr2LEDs(str,name_size,delay_ms);
 }






 
  

