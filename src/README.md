# Hardware Discovery
Tough stuff but we made it. A big part of this was just LLM but I also wrote some code and pressed the buttons :)

## Toolchain Setup
Still using the original one from [main](https://github.com/Imnotndesh/embedded-move-demo/blob/main/platformio.ini)

## Button Hardware Discovery (P2 Mapping)
Through systematic live monitoring of all available I/O ports (P0, P1, P3, P4, P5), the mapping for the pushbuttons was discovered:

The user buttons S1, S2, S3, and S4 are wired to Port 2 (P2), which is typically used for address lines (from schematic) but repurposed here for user input.

| Button | Pin (Confirmed) | Hex Value Change | Logic |
|--------|----------------|------------------|-------|
| S1 | P2.0 | FF to FE | Active Low (0 when pressed) |
| S2 | P2.1 | FF to FD | Active Low (0 when pressed) |
| S3 | P2.2 | FF to FB | Active Low (0 when pressed) |
| S4 | P2.3 | FF to F7 | Active Low (0 when pressed) |

The S5 switch did not register on any monitored port (P0-P5) and remains unmapped. It is likely wired to an unmonitored specialized port (P6) or is connected via an external multiplexing chip.

## Implementation Logic
The current `src/main.c` implements the following clean logic for reading the buttons:

### Port Configuration
The code explicitly sets the relevant Port 2 pins (P2.0 - P2.3) to Input Only / High Impedance Mode (Mode 3) using the STC-specific Port Mode registers (P2M0 and P2M1). This ensures clean readings without signal interference.

```c
P2M0 = 0x0F; // Sets P2.0, P2.1, P2.2, P2.3 to Input Mode (Mode 3, Part 1)
P2M1 = 0x0F; // Sets P2.0, P2.1, P2.2, P2.3 to Input Mode (Mode 3, Part 2)
```

### Input Polling and Debouncing
The main loop checks for the Active Low state (e.g., `if (S_PIN == 0)`). Once a press is detected, the code enters a tight `while (S_PIN == 0)` loop to wait for the button to be released. This acts as hardware debouncing, preventing a single physical press from being registered multiple times.

## Whats Next?
Cheeky understanding demo