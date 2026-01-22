# Lab 4 - MSP430 System Programming: UART Communication & ADC

University lab implementing a layered software architecture (BSP, HAL, API, App) on MSP430 microcontrollers. The project focuses on UART (Universal Asynchronous Receiver/Transmitter) communication between the microcontroller and a PC, combined with ADC (Analog-to-Digital Converter) measurements and FSM management.

# Code Structure

The project follows a generic layered architecture to ensure portability:

    BSP (Board Support Package): Hardware configuration (UART USCI_A0, ADC12, Buzzer, LCD, GPIO).

    HAL (Hardware Abstraction Layer): Drivers for UART (Tx/Rx interrupts), ADC12, and LCD control.

    API: High-level functions for string processing, delay management, and menu display.

    APP: Main FSM logic managed by UART RX Interrupts. Incoming characters are classified as Commands (State selection) or Data (Parameters).

    PC Side: A script (Python/Matlab/C++) acting as a terminal to send commands and visualize received data.

# States Implementation

The system is controlled via a PC Terminal Menu. Sending a number (1-8) triggers the corresponding state in the FSM:

State 1: LCD Counter

    Action: Displays an incrementing integer counter on the LCD.

    Timing: Updates with a configurable delay of X [ms] (Default: 500ms).

    Memory: Preserves the last count value when switching states. 

State 2: Tone Generator (Buzzer)

    Action: Plays a circular series of frequencies on the Buzzer.

    Sequence: {1kHz,1.25kHz,1.5kHz,1.75kHz,2kHz,2.25kHz,2.5kHz}.

    Timing: Switches tones every X [ms]. 

State 3: Configuration (Set Delay)

    Action: Receives a new delay value (X) from the user via UART.

    Behavior: The system waits for ASCII input terminated by ENTER, parses the string to an integer, and updates the global delay variable. 

State 4: Potentiometer Voltage (ADC)

    Action: Measures the analog voltage (0V−3.5V) on pin P1.3.

    Display: Prints the 3-digit voltage value (Volts) dynamically on the LCD. 

State 5: TX Message Trigger

    Action: Sends a predefined string to the PC upon a physical button press.

    Trigger: Pressing PB1 on the development board.

    Output: Transmits "I love my Negev" via UART to the PC terminal. 

State 6: Clear LCD

    Action: Clears the LCD screen and resets the counter variable (from State 1) to zero.

State 8: Sleep Mode

    Default State: System enters LPM (Low Power Mode).

    Behavior: CPU sleeps until a new UART interrupt (Menu selection) or GPIO interrupt occurs.

Real-Time assignment:

State 7 implementation (replaces standard Menu display):
State 7: Real-Time TX Trigger

    Action: Sends a specific string to the PC and echoes it on the terminal.

    Behavior: After selecting this state, on a button press, the system transmits:

        "I love my Negev"

    Constraint: Real-Time Task. Requires handling GPIO interrupts within the UART FSM context.
