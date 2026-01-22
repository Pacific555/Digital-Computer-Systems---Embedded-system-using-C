# DigitalComputerLab3
This project implements a system that includes an MSP430 microcontroller and is based on Simple FSM.

**The topic of this Lab is DMA.**

    a basic requirement of this lab is to define the MxN matrix of chars. (array of strings)
    
 <img width="246" alt="image" src="https://github.com/Orisadek/DigitalComputerLab3/assets/43981934/5bf0034a-f053-4c83-ae68-39dff4027137">


With each button press, we enter a state:

- PB0 (state1):
 	 Create an idiom recorder, for that, we will get input from the user (using a keypad) and save the result in an array named "idiom_recorder".
   Each input will be no more than 32 chars.
  
   Keypad definition:
  
  <img width="226" alt="image" src="https://github.com/Orisadek/DigitalComputerLab3/assets/43981934/1099d1cd-cabf-4ea5-b441-dff0ea71ee95">

- PB1 (state2):
   Do a swap of lines I,j in the matrix using only DMA, it will be done by copying the matrix and changing the copy (the function gets the matrix and two indexes).
  After the swap the updated matrix will be shown on the LCD with the option to scroll down using the keypad (with any chosen key).
  
- PB2 (state3):
   using only DMA light single led and shifting it from left to right in a cycle with a delay of 500ms (Trigger of TimerB).

## Sleep Mode (state0):
state0 is in sleep mode (LPM0).

## Real-Time assignment: 
add state4 as follows,

- PB3 (state4):
Do a mirroring to the string:

      "Google Colaboratory is a free Jupyter notebook environment that runs on Google’s cloud servers, letting the user leverage backend hardware like GPUs and TPUs"

using DMA and printing it on the LCD

