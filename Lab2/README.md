  # DigitalComputerLab2
This project implements a system that includes an MSP430 microcontroller and is based on Simple FSM.
With each button press, we enter a state:

- PB0 (state1):
 	 create a frequency counter to measure f<sub>in</sub> which is an outside clock from the scope and enter to P2.4. the value we found we show on the LCD dynamically and without refreshing the 
   LCD, the frequency will be between 20Hz to 20kHz.
  
  <img width="269" alt="image" src="https://github.com/Orisadek/DigitalComputerLab2/assets/43981934/3d346c20-adb6-43d0-9814-89d6047ddd70">

- PB1 (state2):
   create a clock that counts until one minute (in space of one sec) and then back to zero sec and shows on the LCD.
  
  <img width="260" alt="image" src="https://github.com/Orisadek/DigitalComputerLab2/assets/43981934/b415852f-581f-43d1-9740-28403d41cdbf">

- PB2 (state3):
   Create a Tones generator from an outside signal v<sub>in</sub> that is created in the scope, using the ADC we transform v<sub>in</sub> to f<sub>out</sub>. the frequency we get is the tone.
   The actual Tones come out of the Buzzer we connect to P2.2
  
  <img width="303" alt="image" src="https://github.com/Orisadek/DigitalComputerLab2/assets/43981934/929c6fa9-ef37-4256-a466-a7e35d9a6c3d">

## Sleep Mode (state0):
state0 is in sleep mode (LPM0).

## Real-Time assignment: 
add state4 as follows,

- PB3 (state4):
	print on the LCD the first and last names in a delay of one sec.

FSM:

<img width="521" alt="image" src="https://github.com/Orisadek/DigitalComputerLab2/assets/43981934/2f8ddb22-5ac7-4894-b001-38a7cd6b274c">
