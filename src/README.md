## 1. Summary

| Feature | Specification | Status |
| :--- | :--- | :--- |
| **Microcontroller** | STC89C52RC (8051 Core) | Confirmed |
| **Communication** | UART @ 9600 Baud | Stable (TX/RX Implemented) |
| **Motor Control Pins** | Port 2 (P2.0 - P2.3) | Confirmed Output |
| **Motor Logic** | Active Low (0 = ON) | Confirmed |
| **Speed Control** | Time/Duration-based (No PWM) | Finalized |

> Couldn't figure out pwm since limited documentation and time

## 2. UART Receive Implementation

The code relies on continuously polling the UART status register to detect and process incoming commands. This mechanism turns the serial connection into a responsive command link.

### Explanation of ``process_command()``

The function below implements **Polling** for the receiver, checking the status of the **SCON** register's **RI** bit. If the bit is high, a new byte is loaded from the **SBUF** register, and the flag is cleared to prepare for the next byte.

```c
// Function to check for and execute received serial commands
void process_command()
{
    unsigned char command;
    
    // Check if the Receive Interrupt flag (RI) is set (a byte has been received)
    if (SCON & 0x01) // RI flag is SCON.0 (value 0x01)
    {
        SCON &= ~0x01; // Clear the RI flag
        command = SBUF;  // Read the received byte
        
        // Execute motor function based on the command character
        switch (command)
        {
            // ... cases 'f', 'b', 'l', 'r', 's'
        }
    }
}
```
### Command Mapping Table

The following table shows the final mapping of the received command characters (from the laptop/DualShock 4) to the motor control functions.

| Command Character | Function Executed | Robot Action | P2 Output State (P2.0/1/2/3) | 
| :--- | :--- | :--- | :--- | 
| **`f`** | ``move_forward()`` | Straight Forward | 0 / 1 / 1 / 0 | 
| **`b`** | ``move_back()`` | Straight Backward | 1 / 0 / 0 / 1 | 
| **`l`** | ``move_left()`` | **Spin Left** (Pivot in place) | 0 / 1 / 0 / 1 | 
| **`r`** | ``move_right()`` | **Spin Right** (Pivot in place) | 1 / 0 / 1 / 0 | 
| **`s`** | ``move_stop()`` | **Stop** (All Pins HIGH) | 1 / 1 / 1 / 1 |

> Current code in The car is of the above