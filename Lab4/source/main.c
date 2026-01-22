#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer
#define ascii_offset 48
#define id_size 9



enum FSMstate state;
enum SYSmode lpm_mode;

void main(void){
  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
    while(1){
	switch(state){
	case state0:
        enterLPM(lpm_mode);
		break;
		 
	  case state1:
	      state1_func();
		break;
		 
	  case state2:
	      state2_func();
		break;
		
	  case state3:
	      state3_func();
	    break;
	  case state4:
	      state4_func();
	      state = state0;
	    break;
	  case state5:
	      state5_func();
	  break;
	  case state6:
	    state6_func();
	    state = state0;
	 break;
	  case state7:
	    state7_func();
	    state = state0;
	  break;
	  case state8:
	          state = state0;
	      break;

	}
  }
}
  
  
  
  
  
  
