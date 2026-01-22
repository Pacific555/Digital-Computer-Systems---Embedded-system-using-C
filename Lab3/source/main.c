#include <header/api.h>    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer

enum FSMstate state;
enum SYSmode lpm_mode;
extern char idiom_recorder[32];


void main(void){
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET


  sysConfig();
  lcd_init();
  lcd_clear();
  
  while(1){
	switch(state){
	  case state0:                // goes to sleep
                enterLPM(lpm_mode);
		break;
	
      case state1:
          idiom_record();
          state = state0;
          break;
                
      case state2:
          mirror_string();
          break;
		 
	  case state3:
	      Print_Arr_on_Leds();
	      break;
                
      case state4:                //real time task
          swap_half_string();
          break;
		
	}
  }
}
  
  
  
  
  
  
