# `STC89C52` Serial Communication (UART)
This document explains the foundation of the C code used to establish reliable serial communication (UART) on the `STC89C52` micro-controller, targeting a baud rate of 9600.

## 1. Hardware Register Definitions
The first section of the C code manually defines the Function Registers needed for timing and serial communication.

|Register|Hex Address|Purpose|
|---|---|---|
|`TCON`|`0x88`|**Timer Control:** Used to start and stop timers (`TR1` flag).|
|`TMOD`|`0x89`|**Timer Mode:** Used to configure Timer 1 (for baud rate generation).|
|`TH1`|`0x8d`|**Timer 1 High:** Stores the reload value for the baud rate calculation.|
|`SCON`|`0x98`|**Serial Control:** Configures the UART mode and manages flags.|
|`SBUF`|`0x99`|**Serial Buffer:** The hardware register used to send data (bytes).|

## 2. The UART Initialization (`init_uart_9600`)

This function contains the core setup for configuring the Serial Port to run at 9600 baud, relying on the common **11.0592 MHz crystal**.

| Code Line      | Explanation                                                                                                                                                             |
| -------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `TMOD`         | =` 0x20`;                                                                                                                                                               |
| `SCON = 0x50;` | **Sets Serial Port Mode 1** (8-bit UART, variable baud rate). The `0x50` also sets the `REN` (Receive Enable) bit, ensuring we can receive data if needed.              |
| `TH1 = 0xFD;`  | **Sets the reload value for Timer 1.** The value `0xFD` (decimal 253) is the required value to generate a perfect 9600 baud signal when dividing the 11.0592 MHz clock. |
| `TCON          | = 0x40;`                                                                                                                                                                |
| `SCON          | = 0x02;`                                                                                                                                                                |

## 3. The `printf` Bridge (`putchar`)

The standard C function `printf()` is used, but it relies on a lower-level function named `putchar()` to handle the actual hardware communication. We must implement `putchar()` ourselves to direct the output to the `SBUF` register.

| Code Line                | Explanation                                                                                                                                                                                                     |
| ------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `while(!(SCON & 0x02));` | **Wait Loop:** Halts execution until the `TI` (Transmit Interrupt) flag is set. This flag indicates that the hardware has finished sending the previous byte and the `SBUF` register is ready for the next one. |
| `SCON &= ~0x02;`         | **Clear Flag:** Clears the `TI` flag.                                                                                                                                                                           |
| `SBUF = c;`              | **Send Data:** Moves the character (`c`) into the Serial Buffer Register (`SBUF`). The hardware immediately takes this byte and begins sending it out of the `TXD` pin.                                         |

## 4. Delay Function (`delay_ms`)

This is a simple software-based "spin-lock" delay. The number `114` in the inner loop is an experimentally determined value to produce approximately a 1 millisecond delay when the chip is running at 11.0592 MHz.

- **Purpose:** Provides a predictable pause between actions (like sending strings or toggling motors) without using complex timers or interrupts.
- **Accuracy:** This delay is blocked if interrupts occur, making it suitable only for non-critical timing.