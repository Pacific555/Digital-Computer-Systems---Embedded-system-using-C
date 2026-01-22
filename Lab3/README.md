# Lab 3 - MSP430 System Programming: DMA & Data Structures

University lab implementing a layered software architecture (BSP, HAL, API, App) on MSP430 microcontrollers. The project focuses on the Direct Memory Access (DMA) controller for efficient data transfer, signal generation, and memory manipulation without CPU intervention.
# Code Structure

The project follows a generic layered architecture to ensure portability:

    BSP (Board Support Package): Hardware configuration (DMA channels, LCD, Keypad GPIO).

    HAL (Hardware Abstraction Layer): Drivers for DMA controller (Block/Single transfer), LCD (4-bit mode), and Keypad scanning.

    API: High-level functions for string mirroring and Keypad input processing.

    APP: Main FSM logic managing DMA triggers and state transitions.

# States Implementation

Button-driven states managed by a "Simple FSM" architecture, utilizing the DMA controller for background operations:

PB0 (State 1): Idiom Recorder

    Action: Records a user-input string via the Keypad and displays it on the LCD.

    Behavior:

        Inputs characters from a 4x4 Keypad.

        Stores up to 32 characters in the idiom_recorder buffer.

        Real-time echoing of typed characters to the LCD.

    Constraint: Interruptible. Ends when a different state button is pressed.

PB1 (State 2): DMA String Mirroring

    Action: Mirrors a selected string from a static database using DMA Block Transfer.

    Behavior:

        User selects an index via Keypad to choose a string from data_base.

        DMA Configuration: Configured to reverse the string (Source increment, Destination decrement).

        Output: The mirrored string (strMirror) is printed to the LCD.

    Constraint: Non-interruptible (Atomic). Must complete the mirroring and display process.

PB2 (State 3): LED Pattern (DMA + Timer)

    Action: Displays a specific array sequence [128, 224, 32, 126, 83, 44, 253, 113, 160] on the LEDs.

    Behavior:

        Uses DMA Single Transfer mode.

        Trigger: Hardware trigger via TimerB (TACCR2_CCIFG) every 500ms.

        Transfers data directly from memory to the Port Output Register (P2OUT) without CPU overhead.

    Constraint: Non-interruptible (Atomic). Runs until the array cycle is complete.

PB3 (State 4): Real-Time Google Colab Mirroring

    Action: Mirrors and prints a long description string on the LCD using DMA.

    Data Source:

        "Google Colaboratory is a free Jupyter notebook environment that runs on Google’s cloud servers, letting the user leverage backend hardware like GPUs and TPUs"

    Behavior:

        Uses DMA to reverse the order of characters in memory.

        Prints the processed string to the LCD with scrolling support if necessary.

    Constraint: Real-Time Task.

Sleep Mode (State 0)

    Default State: System enters LPM (Low Power Mode) when idle.

    Behavior: DMA channels disabled (unless active in background). CPU sleeps until a new Port interrupt occurs.
