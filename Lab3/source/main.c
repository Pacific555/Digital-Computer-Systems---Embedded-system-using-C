#include  "../header/api.h"    		// private library - API layer
#include  "../header/app.h"    		// private library - APP layer
//#include  <string.h>
#define ascii_offset 48



enum FSMstate state;
enum SYSmode lpm_mode;


void main(void){
  char recorder_idiom[33] = {0} ;
  char data_matrix_in[N][M] = {
  "An apple a day keeps the doctor away",
  "climb on the bandwagon",
  "Dot the i's and cross the t's",
  "He who pays the piper calls the tune",
  "The pen is mightier than the sword",
  "The pot calling the kettle black",
  "shed crocodile tears",
  "Close but no cigar",
  "Cut from the same cloth",
  "Strike while the iron's hot"
  };
  char data_matrix_out[N][M];
  int ledsArr [8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
  char str[158] = "Google Colaboratory is a free Jupyter notebook environment that runs on Google�s cloud servers, letting the user leverage backend hardware like GPUs and TPUs";
  char strMirror[158]  = {0};

  state = state0;  // start in idle state on RESET
  lpm_mode = mode0;     // start in idle state on RESET
  sysConfig();
  
  while(1){
	switch(state){
	  case state0:
        enterLPM(lpm_mode);
		break;
		 
	  case state1:
	      state1_func(recorder_idiom);
	   //   lcd_puts(recorder_idiom);
		break;
		 
	    case state2:
	        state2_func(2,5,data_matrix_in, data_matrix_out);
	        state = state0;
		break;
		
	  case state3:
	      state3_func(ledsArr);
	    break;

	  case state4:
	      state4_func(str, strMirror, 158);
	      state = state0;
	    break;
	}
  }
}
  
  
  
  
  
  
