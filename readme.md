# LED Challenge

## Overview
This project demonstrates a timer-based LED toggle on the NXP FRDM-K64F board.  
It is based on the **FRDM-K64F SDK LED blinky example**.

## Features
- Uses **LPTMR0 timer interrupt** to toggle the LED instead of a busy-wait loop.  
- Board initialization (`clock_config`, `pin_mux`, `board`) handled by SDK.  
- Demonstrates proper use of peripherals, interrupts, and low-level hardware control.  

## Dependencies
- ARM GCC Toolchain (arm-none-eabi)
- NXP FRDM-K64F SDK
- Make
- CMake

## Build Instructions
1. Ensure sdk path environment variable is set properly 
```export SDK_PATH=/path/to/SDK```
2. Change to build directory
```cd armgcc```
3. Build the project
```./build_debug.sh```
4. Flash the binary 
```./flash.sh```
