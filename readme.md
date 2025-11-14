# LED Challenge

## Overview
This project demonstrates a timer-driven LED control application on the NXP FRDM-K64F board.
It is derived from the **FRDM-K64F SDK LED blinky example**, but replaces the busy-wait loops with proper hardware timers and interrupt-driven scheduling

Two independent LED patterns are implemented using a free-running LPTMR0:
- LED1 (Red): Toggles every 65,000 timer counts via interrupt
- LED2 (Green): Flashes 5 times every 3 seconds using software timing in the main loop

## Features
- Board initialization (`clock_config`, `pin_mux`, `board`) handled by SDK.
- Demonstrates proper use of peripherals, interrupts, and low-level hardware control.

## Clock Config
- Timer: LPTMR0
- Clock Source: LPO (1 kHz)
- Prescaler: Disabled (bypass enabled)
- Mode: Free-running, 16-bit counter

## Dependencies
- ARM GCC Toolchain (arm-none-eabi)
- NXP FRDM-K64F SDK
- Make
- CMake

## Build Instructions
1. Set SDK path
```export SDK_PATH=/path/to/SDK```
2. Enter the build directory
```cd armgcc```
3. Build the project
```./build_debug.sh```
4. Flash the binary
```./flash.sh```

## Debug
Read debug UART console at:
- Baud: 115200
- 8-N-1, no flow control

