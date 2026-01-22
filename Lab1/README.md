# Lab 1 - MSP430 System Programming: GPIO, Interrupts & FSM

University lab implementing a layered software architecture (BSP, HAL, API, App) on MSP430 microcontrollers. The project demonstrates an interrupt-driven Finite State Machine (FSM) to manage GPIO, binary counting, PWM signal generation, and power management via Low Power Modes (LPM).

# Code Structure

The project follows a generic layered architecture to ensure portability between MSP430x2xx and MSP430x4xx families:

    BSP (Board Support Package): Hardware configuration (GPIO settings).

    HAL (Hardware Abstraction Layer): Drivers for LEDs, Switches, and PWM.

    API: High-level interface functions.

    APP: Main FSM logic and application flow.

# States Implementation

Precise button-driven states managed by a "Simple FSM" architecture:

PB0 (State 1): Binary Counter

    Action: Displays a binary count on an 8-LED array.

    Behavior:

        Odd press: Counts UP (binary).

        Even press: Counts DOWN (binary).

    Timing: 0.5s delay between counts.

    Duration: Runs for 10 seconds total.

    Constraint: Non-interruptible (Atomic). Must complete the 10s cycle before changing states.

    Memory: Resumes counting from the last stored value.

PB1 (State 2): LED Shifting

    Action: Shifts a single lit LED from right to left across the array.

    Timing: 0.5s delay between shifts.

    Duration: Runs for 7 seconds total.

    Constraint: Non-interruptible (Atomic).

    Memory: Resumes shifting from the last position.

PB2 (State 3): PWM Generation

    Action: Generates a PWM signal on pin P2.7 (verified via scope).

    Behavior:

        Odd press: 1 kHz frequency with 75% Duty Cycle.

        Even press: 2 kHz frequency with 25% Duty Cycle.

    Constraint: Interruptible. This state runs indefinitely until interrupted by another button press (State change).

PB3 (State 4): Real-Time String Display

    Action: Prints the ASCII value of each character in a predefined string on the LEDs.

    Timing: 250ms latency between characters.

    Constraint: Non-interruptible (Atomic). The full string must be displayed before the system can accept new inputs or change states.

Sleep Mode (State 0)

    Default State: System enters LPM (Low Power Mode) when idle.

    Behavior: LEDs are turned off. CPU sleeps to save power until a new Port 2 interrupt (button press) occurs.
