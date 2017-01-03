# ECEN-5013-Final-Project
Source codes for the ECEN 5013 final project in the Fall semester, 2016

This project is to explore the way to develop a hardware abstraction layer for the KL25Z, and build flexible APIs for different modules on the KL25Z so that one who has no hardware knowledge of the board can still be able to control the board for desired applications.

It also used a super loop to emulate the behavior of a RTOS. See this for the program flow:
https://drive.google.com/open?id=0B4HWPEcAmzwjZGFTZHBxcndBX1E

Software concept implemented in this project includes:
- Structure/Enumeration/Preprocessor Macros;
- Inline Keyword;
- Interrupts/Callbacks/Function Pointer;
- FIFO/State Machine;
- Event/Semaphore/Mutex;
- Version Control;
- Hardware Abstraction/Software APIs;
