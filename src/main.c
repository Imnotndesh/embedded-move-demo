#include <stdio.h> 
#include <stdbool.h>

// --- UART Register Definitions (For Console Debugging) ---
__sfr __at(0x88) TCON;
__sfr __at(0x89) TMOD;
__sfr __at(0x8d) TH1;
__sfr __at(0x98) SCON;
__sfr __at(0x99) SBUF;

// --- Port Mode Configuration Registers ---
__sfr __at(0xA2) P2M0; 
__sfr __at(0xA3) P2M1; 
__sfr __at(0x91) P1M0; // Port 1 Mode
__sfr __at(0x92) P1M1; // Port 1 Mode

// --- Button Register Definitions (P2.0 - P2.3) ---
__sbit __at(0xA0 + 0) S1_PIN; // P2.0
__sbit __at(0xA0 + 1) S2_PIN; // P2.1
__sbit __at(0xA0 + 2) S3_PIN; // P2.2
__sbit __at(0xA0 + 3) S4_PIN; // P2.3

// --- Motor Control Pin Definitions (Assuming P1.0 - P1.5) ---
__sbit __at(0x90 + 0) M1_A; // P1.0 - Front Left Direction A
__sbit __at(0x90 + 1) M1_B; // P1.1 - Front Left Direction B
__sbit __at(0x90 + 2) M2_A; // P1.2 - Front Right Direction A
__sbit __at(0x90 + 3) M2_B; // P1.3 - Front Right Direction B
__sbit __at(0x90 + 4) ENA;  // P1.4 - Motor Enable A/Left Side
__sbit __at(0x90 + 5) ENB;  // P1.5 - Motor Enable B/Right Side

// --- Helper Functions ---

// Initializes UART for 9600 baud, assuming 11.0592 MHz crystal.
void init_uart_9600() 
{
    TMOD |= 0x20; 
    SCON = 0x50;  
    TH1 = 0xFD; 
    TCON |= 0x40; 
    SCON |= 0x02;               
}

// Debounce and general delay function.
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 114; j++); 
    }
}

// printf support function.
int putchar(int c)
{
	while(!(SCON & 0x02)); 
	SCON &= ~0x02;         
	SBUF = c;              
	return (c);
}

// --- Motor Actions ---

void motor_enable()
{
    // Set enable pins HIGH (active)
    ENA = 1; 
    ENB = 1; 
}

void move_stop()
{
    M1_A = 0; M1_B = 0; // Motor 1 (Left) Stop
    M2_A = 0; M2_B = 0; // Motor 2 (Right) Stop
    printf("STOP\n");
}

void move_forward()
{
    // Both motors spin forward (M_A=1, M_B=0)
    M1_A = 1; M1_B = 0; 
    M2_A = 1; M2_B = 0; 
    printf("FORWARD\n");
}

void move_backward()
{
    // Both motors spin backward (M_A=0, M_B=1)
    M1_A = 0; M1_B = 1; 
    M2_A = 0; M2_B = 1; 
    printf("BACKWARD\n");
}

void turn_left()
{
    // Left motor (M1) backward, Right motor (M2) forward
    M1_A = 0; M1_B = 1; 
    M2_A = 1; M2_B = 0; 
    printf("TURN LEFT\n");
}

void turn_right()
{
    // Left motor (M1) forward, Right motor (M2) backward
    M1_A = 1; M1_B = 0; 
    M2_A = 0; M2_B = 1; 
    printf("TURN RIGHT\n");
}


// --- Main Program ---

void main(void)
{
    // 1. Configure Input Ports (P2 for buttons)
    P2M0 = 0x0F; // P2.0-P2.3 to Input Mode (Mode 3, Part 1)
    P2M1 = 0x0F; // P2.0-P2.3 to Input Mode (Mode 3, Part 2)

    // 2. Configure Output Ports (P1 for motors)
    P1M0 = 0x00; // P1.0-P1.5 to Push-Pull Mode (Mode 1, Part 1) - Default is Push-Pull
    P1M1 = 0x00; // P1.0-P1.5 to Push-Pull Mode (Mode 1, Part 2)

    // 3. Initialize peripherals
    init_uart_9600();
    motor_enable(); 
    
    printf("Robot Control Initialized. Drive with S1-S4.\n");
    delay_ms(100); 

    while (1)
    {
        // S1: FORWARD
        if (S1_PIN == 0) { 
            move_forward(); 
            while (S1_PIN == 0) delay_ms(10); // Hold during press
            move_stop();
        }

        // S2: BACKWARD
        else if (S2_PIN == 0) { 
            move_backward(); 
            while (S2_PIN == 0) delay_ms(10); // Hold during press
            move_stop();
        }

        // S3: TURN LEFT
        else if (S3_PIN == 0) { 
            turn_left(); 
            while (S3_PIN == 0) delay_ms(10); // Hold during press
            move_stop();
        }

        // S4: TURN RIGHT
        else if (S4_PIN == 0) { 
            turn_right(); 
            while (S4_PIN == 0) delay_ms(10); // Hold during press
            move_stop();
        }

        // Default: STOP
        else {
            move_stop();
        }
    }
}