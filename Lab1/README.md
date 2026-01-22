# DigitalComputerLab1

This project implements a system that includes MSP430 microcontroller and is based on Simple FSM.
With each button press, we enter a state:

- PB0 (state1):
 	 The array values are displayed on LEDs with a delay of 0.5 seconds between each number in the array.

- PB1 (state2):
  	Light one led and shifted it from right to left with a delay of 0.5 sec between each shift.

- PB2 (state3):
        Create a PWM signal with a frequency of 4kHz and a duty cycle of 75.%
        this is the only state which allows a shift in the state while still in the current one.

## Sleep Mode (state0):
state0 is in sleep mode (LPM0) and every state except the state3 returns to sleep after the state ends.

## Real-Time assignment: 
add state4 as follows,

- PB3 (state4):
	print on LEDs the ASCII value of each char in a string with a latency of 250 ms


	
FSM:

<img width="361" alt="image" src="https://github.com/Orisadek/DigitalComputerLab1/assets/43981934/d3ceb60b-15a5-4d10-8bdf-be536c73fcaf">


