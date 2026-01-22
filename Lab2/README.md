# Lab 2 - MSP430 System Programming: Timers, ADC & DAC

University lab implementing a portable layered software architecture (BSP, HAL, API, App) on MSP430 microcontrollers. The project demonstrates an interrupt-driven Finite State Machine (FSM) to manage Timers (Input Capture/Output Compare), ADC measurements, LCD display control, and signal generation.
# Code Structure

The project follows a generic layered architecture to ensure portability between MSP430x2xx and MSP430x4xx families:

    BSP (Board Support Package): Hardware configuration (GPIO, Clock calibration, LCD pins).

    HAL (Hardware Abstraction Layer): Drivers for LCD, Timers (A0/A1), ADC10, and Signal generation.

    API: High-level interface functions (e.g., string printing to LCD, frequency calculation).

    APP: Main FSM logic and application flow using integer arithmetic (Q-Format).

# States Implementation

Button-driven states managed by a "Simple FSM" architecture, utilizing hardware timers and analog interfaces:
PB0 (State 1): Frequency Counter

    Action: Measures an external square wave signal frequency (fin​) connected to the Input Capture pin.

    Display: Shows the measured frequency in Hz on the LCD.

    Behavior:

        Uses Timer Input Capture mode for high precision.

        Supports frequency range: 50Hz - 20kHz.

        Updates the LCD value dynamically.

    Constraint: interruptible. Requires a signal generator input.

PB1 (State 2): Stopwatch

    Action: Implements a digital stopwatch displayed on the LCD in MM:SS format.

    Behavior:

        SW0 High ('1'): Starts/Resumes counting.

        SW0 Low ('0'): Pauses counting.

        Uses Timer Interrupts (Up/Down mode) for precise 1-second timing.

    Memory: Resumes from the last stored time value when toggled or re-entered.

PB2 (State 3): Tone Generator

    Action: Generates a PWM audio signal on the Buzzer based on Potentiometer input.

    Behavior:

        Input: Reads analog voltage (Vin​) from the potentiometer using ADC10.

        Processing: Maps voltage (0V - 3.3V) to frequency (1kHz - 2.5kHz) using a linear transformation formula.

        Output: Generates the calculated frequency using Timer Output Compare (PWM) on the buzzer pin.

    Constraint: Interruptible. Runs continuously, updating pitch in real-time until a state change occurs.

PB3 (State 4): Real-Time Name Display

    Action: Prints the First and Last names on the LCD.

    Timing: Updates with a delay of 1 second.

    Constraint: Real-Time Task.

Sleep Mode (State 0)

    Default State: System enters LPM (Low Power Mode) when idle.

    Behavior: Peripherals are disabled or low-power. CPU sleeps until a new Port interrupt (button press) occurs.
