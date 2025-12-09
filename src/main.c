#include <stdio.h> 
#include <stdbool.h>

__sfr __at(0x88) TCON;
__sfr __at(0x89) TMOD;
__sfr __at(0x8d) TH1;
__sfr __at(0x98) SCON;
__sfr __at(0x99) SBUF;

__sfr __at(0xA2) P2M0; 
__sfr __at(0xA3) P2M1; 

__sbit __at(0xA0 + 0) S1_IN; 
__sbit __at(0xA0 + 1) S2_IN; 
__sbit __at(0xA0 + 2) S3_IN; 
__sbit __at(0xA0 + 3) S4_IN; 


void init_uart_9600() 
{
    // Configure Timer 1 for 9600 baud
    TMOD |= 0x20; 
    TH1 = 0xFD; 
    TCON |= 0x40; 
    
    // Configure Serial Port Mode 1 (8-bit UART) and set REN=1 (Receive Enable)
    SCON = 0x50 | 0x10; // 0x50 (Mode 1) | 0x10 (REN=1)
    
    // Set TI flag to 1 (ready to send)
    SCON |= 0x02;               
}

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 114; j++); 
    }
}

int putchar(int c)
{
	while(!(SCON & 0x02)); 
	SCON &= ~0x02;         
	SBUF = c;              
	return (c);
}

void move_forward() 
{
    S1_IN = 0; 
    S2_IN = 1; 
    S3_IN = 1; 
    S4_IN = 0; 

    printf("FORWARD\n");
}

void move_back() 
{
    S1_IN = 1; 
    S2_IN = 0; 
    S3_IN = 0; 
    S4_IN = 1; 
    
    printf("BACKWARD\n");
}

void move_left()
{
    S1_IN = 0; 
    S2_IN = 1; 
    S3_IN = 0; 
    S4_IN = 1; 

    printf("SPIN LEFT\n");
}

void move_right()
{
    S1_IN = 1; 
    S2_IN = 0; 
    S3_IN = 1; 
    S4_IN = 0; 

    printf("SPIN RIGHT\n");
}

void move_stop() 
{
    S1_IN = 1;
    S2_IN = 1;
    S3_IN = 1;
    S4_IN = 1;
    
    printf("STOP\n");
}

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
            case 'f':
                move_forward();
                break;
            case 'b':
                move_back();
                break;
            case 'l':
                move_left();
                break;
            case 'r':
                move_right();
                break;
            case 's':
                move_stop();
                break;
            default:
                // Ignore errors
                break;
        }
    }
}


void main(void)
{
    P2M0 &= ~0x0F; 
    P2M1 &= ~0x0F; 

    init_uart_9600();
    
    printf("--- Robot Control Ready (Awaiting Command From controller) --- \n");
    
    // Ensure motors are initially stopped
    move_stop();
    delay_ms(100); 

    while (1)
    {
        // Continuously check for new commands
        process_command();
        
        // Small delay to sleep the CPU while waiting for new commands
        delay_ms(10);
    }
}