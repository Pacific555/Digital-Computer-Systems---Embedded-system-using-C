# DigitalComputerLab4
This project implements a system that includes a client-side (PC) and a server (MSP430 microcontroller). The PC side includes a menu. 
The server side includes a microcontroller.
The communication between both sides is made with UART. 

**The topic of this Lab is UART.**
The menu includes the following options:
  1. Blink RGB LED, color by color with a delay of x. (state1)
  2. Count up onto the LCD screen with a delay of x, the value of the count is saved between states.  (state2)
  3. Circular tone series via Buzzer with a delay of x, the series of the tones is [1,1.25,1.5,1.75,2,2.25,2.5] all in units of kHz. (state3)
  4. Set delay time x. (state4)
  5. Print LDR 3-digit value onto LCD (in volts) (dynamically). (state5)
  6. Clear the LCD screen and initialize the value of the count in state 2. (state6)
  7. Show the menu on the PC screen. (not an C state, only in PC side)
  8. Sleep mode. (state0)

<img width="368" alt="image" src="https://github.com/Orisadek/DigitalComputerLab4/assets/43981934/4f8c9165-c49a-4825-8e20-3fd04f512730">

> [!IMPORTANT]
> The default value of x is 500 and the units are ms

## Real-Time assignment: 
add option as follows,

- state7:
  After we chose the state and on button press, we will send "I love my Negev" to the PC and show it on the terminal.

- FSM:

  <img width="324" alt="image" src="https://github.com/Orisadek/DigitalComputerLab4/assets/43981934/6d2c7cf8-1195-4927-b702-1cf8fa5129ad">

  


