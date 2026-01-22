# DCS Final Project - Light Source & Object Proximity Detector System

University final project in the "Digital Computer Structures" (DCS) course. This project implements an advanced Real-Time Embedded System integrating sensors, actuators (Servo), a custom Flash File System, and intelligent UART communication with a PC-based Graphical User Interface (GUI).

The system operates as a smart Slave unit based on the MSP430 microcontroller, performing real-time signal processing and power management, while the PC acts as the Master for heavy data visualization and control.
# Key Features

**PC-Side GUI:** Full system control via a desktop application, featuring real-time radar plots, calibration graphs, and data logging.

**Smart Flash Management:** A custom file system for storing text scripts and calibration data, utilizing dedicated structs to optimize memory usage.

**Power Optimization:** Extensive use of Low Power Modes (LPM) and an efficient communication protocol that minimizes data transmission (e.g., inferring angles instead of sending them).

**Signal Processing:** Implementation of Median and Average filters, along with interpolation algorithms for maximum measurement accuracy.

**Real-Time Integration:** seamless synchronization between Servo movement, Ultrasonic echo capture, and UART transmission.

# System Architecture & FSM States

The system is controlled by a Finite State Machine (FSM), triggered and managed by UART commands from the PC GUI:

### State 1: Object Detector System (Radar Scan)

- **Description:** Performs a full radar scan to detect objects in the environment.

- **Operation:** The Servo motor scans from 0 to 180 degrees.

- **Protocol:** For every degree, a compact 16-bit data packet is sent to the PC.

- **Logic:** The user defines a distance Threshold in the GUI; objects beyond this range are ignored.

- **Output:** The GUI renders a real-time radar image, displays the current angle and distance, and calculates the width of the detected object upon scan completion.

<img width="1057" height="714" alt="object detaction" src="https://github.com/user-attachments/assets/370c20ca-7f37-4394-8361-4705d5caf1ea" />

### State 2: Telemeter

 - **Description:** Point-to-point real-time distance measurement.

- **Operation:** Utilizes the Ultrasonic sensor at a fixed angle to detect the closest object.

- **Output:** Continuous real-time display of the distance to the target.

<img width="1045" height="690" alt="telemeter" src="https://github.com/user-attachments/assets/7710003c-3c44-4a8a-a817-d7cdbbfb9910" />

### State 3: LDR Samples Calibration

- **Significance:** Critical calibration for the Light Dependent Resistor (LDR) to adjust for environmental lighting conditions.

- **Process:**
   - Measures resistance at distances from 5cm to 50cm (in 5cm intervals).

   - The scan is performed twice to ensure accuracy.

   - Memory Management: Results are stored in Flash memory using a unique data structure designed for optimal space efficiency.

- **Processing:** The PC receives the raw data and performs interpolation to generate a calibration curve displayed to the user.
 
<img width="1119" height="755" alt="calibration mode" src="https://github.com/user-attachments/assets/8a763477-5a5e-495e-bce6-ff57b972a9cf" />

### State 4: Light Sources Detector System

- **Description:** Detects and maps light sources in the surrounding space.

- **Operation:** A 180 degrees scan where 2 LDR sensors are sampled (3 samples per sensor per angle using ADC10).

- **MCU Algorithm:** Calculates the Median for each sensor and then Calculates the Average of the two medians.

- **Power Saving Protocol:** The MCU sends only the sample value (2 Bytes) to the PC. The angle is NOT sent - The PC automatically increments the angle counter for each received packet, significantly reducing transmission time and power consumption.

- **PC Processing:**

    - Categorizes sample sequences (Blobs) wider than 4 degrees as "Potential Light Sources".

    - Identifies the local minimum (maximum intensity) within the blob.

    - Calculates the distance using a Look-Up Table (LUT) derived from the calibration.
      
<img width="1054" height="700" alt="light detection" src="https://github.com/user-attachments/assets/c020f4b2-4893-4d8c-8ccf-4f3f39c65153" />

### State 5: Flash File System (Upload via ISR)

- **Description:** Uploads text files and scripts from the PC to the MCU's Flash memory.

- **Capacity:** Supports up to 10 files.

- **Data Structure:** Uses a struct FileEntry (containing: Name, Type, Size, Pointer to the first cell in the memory ). 

- **Transfer Protocol:**

    - Header: MCU receives metadata (Name, Type, Size).

    - Handshake: MCU checks for available space and responds:

        - 'A' (Ack): Space available, ready to receive.

        - 'F' (Full): Flash memory is full.

        - 'E' (Error): Transmission error.

    - Data: Structs are saved in Segment C, and file content is saved sequentially in Segments 1-4. Writing is done directly to Flash, byte-by-byte.

    - Completion: MCU sends 'K' upon successful transfer ("All Data Transmitted").
    - 
<img width="983" height="669" alt="text file uploading" src="https://github.com/user-attachments/assets/d94f5a4a-588a-433f-9d15-a096ec559047" />

###  State 6: Erase Flash

- **Action:** Erases all file content (Data) and management structures (Descriptors) from the Flash memory to free up space and reorganize the file system.

###  State 7: Show Files (LCD Interface)

- **Action:** A standalone interface on the MCU's LCD screen.

- **Functionality:** Allows the user to view the list of files stored in Flash, select a file, and read its content (supports scrolling/paging).

### State 8: Bonus - Light + Objects

- **Description:** Sensor Fusion mode. Combines the Radar Scan (Ultrasonic) and Light Source Detection (LDR) into a single integrated scan pass, providing a comprehensive environmental map.

### State 9: Sleep Mode (LPM)

- **Description:** The default state. The MCU enters LPM (Low Power Mode) to conserve energy, waking up only upon receiving a UART interrupt (Command).

# PC Side GUI

The PC Pyhton application is designed to provide high-level control and data analysis:

- Control Panel: Easy mode selection via buttons.

- Visualization: Real-time Radar Plots and Calibration Graphs.

- File Manager: Interface for selecting and uploading scripts to the MCU.

- Data Logger: Automatically saves session data and results.

# Flash Memory Layout

The system implements a custom memory map for non-volatile storage:

- Segment C: Stores File Descriptors (Metadata structs).

- Segments 1-4: Stores the actual File Content (Data).

- Logic: The write operation is managed intelligently to append data without overwriting existing files, provided there is free space.

## FSM

<img width="707" height="650" alt="FSM" src="https://github.com/user-attachments/assets/c2c66fa1-f47e-400a-b168-5f324991bed7" />

## Block Diagram

<img width="1416" height="637" alt="skech circut system" src="https://github.com/user-attachments/assets/33c4921a-cfae-4483-a2b3-c250e52b8b2d" />


The figure above illustrates the high-level hardware architecture.
The MSP430 microcontroller serves as the central processing unit, interfacing with the sensor array (Ultrasonic & LDRs) and the servo motor for scanning operations.
All gathered data is transmitted in real-time to the PC via a UART bridge for visualization and logging.

## Physical System Overview:

<img width="633" height="467" alt="the system" src="https://github.com/user-attachments/assets/9bb9fd25-6a14-4868-b7c7-d9bb3b771d5e" />

<img width="606" height="453" alt="ultrasonic sensor on top of servo" src="https://github.com/user-attachments/assets/764e414a-87d2-4498-8bf5-ca94a1b83366" />

