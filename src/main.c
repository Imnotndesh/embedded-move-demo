#include <stdio.h> 
#include <stdbool.h>

// UART Register Definitions
__sfr __at(0x88) TCON;
__sfr __at(0x89) TMOD;
__sfr __at(0x8d) TH1;
__sfr __at(0x98) SCON;
__sfr __at(0x99) SBUF;

// Port SFR Definitions 
__sfr __at(0xA0) P2; // Port 2 Register

// Port Mode Configuration Registers
__sfr __at(0xA2) P2M0; 
__sfr __at(0xA3) P2M1; 

// Pin Definitions
__sbit __at(0xA0 + 0) S1_IN; // P2.0 (Right Motor Forward Pin)
__sbit __at(0xA0 + 1) S2_IN; // P2.1 (Right Motor Backward Pin)
__sbit __at(0xA0 + 2) S3_IN; // P2.2 (Left Motor Backward Pin)
__sbit __at(0xA0 + 3) S4_IN; // P2.3 (Left Motor Forward Pin)


void init_uart_9600() 
{
    TMOD |= 0x20; 
    SCON = 0x50;  
    TH1 = 0xFD; 
    TCON |= 0x40; 
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

// Motor Control Functions 

void move_forward() 
{
    // Right Forward (S1=0, S2=1) and Left Forward (S4=0, S3=1)
    S1_IN = 0; 
    S2_IN = 1; 
    S3_IN = 1; 
    S4_IN = 0; 

    printf("Simulating Forward\n");
}

void move_backward() 
{
    // Right Backward (S2=0, S1=1) and Left Backward (S3=0, S4=1)
    S1_IN = 1; 
    S2_IN = 0; 
    S3_IN = 0; 
    S4_IN = 1; 
    
    printf("Simulating Backward\n");
}

void turn_left()
{
    // Right Forward (S1=0, S2=1) and Left Backward (S3=0, S4=1)
    S1_IN = 0; 
    S2_IN = 1; 
    S3_IN = 0; // LEFT BACKWARD
    S4_IN = 1; 

    printf("Simulating Left Spin Turn\n");
}

void turn_right()
{
    // Right Backward (S2=0, S1=1) and Left Forward (S4=0, S3=1)
    S1_IN = 1; 
    S2_IN = 0; // RIGHT BACKWARD
    S3_IN = 1; 
    S4_IN = 0; // LEFT FORWARD

    printf("Simulating Right Spin Turn\n");
}

void move_stop() 
{
    S1_IN = 1;
    S2_IN = 1;
    S3_IN = 1;
    S4_IN = 1;
    
    printf("STOP\n");
}

void main(void)
{
    // Configure P2.0-P2.3 for Push-Pull Output
    P2M0 &= ~0x0F; 
    P2M1 &= ~0x0F; 

    init_uart_9600();
    
    printf("Motor Spin Test \n");
    delay_ms(100); 

    while (1)
    {
        // 1. FORWARD (300ms)
        move_forward();
        delay_ms(300); 

        // 2. STOP (300ms)
        move_stop();
        delay_ms(300); 

        // 3. LEFT SPIN TURN (800ms for dramatic turn)
        turn_left();
        delay_ms(800); 

        // 4. STOP (300ms) `essentially back tbh`
        move_stop();
        delay_ms(300); 
        
        // 5. BACKWARD (300ms)
        move_backward();
        delay_ms(300); 

        // 6. STOP (300ms)
        move_stop();
        delay_ms(300); 
        
        // 7. RIGHT SPIN TURN (800ms for dramatic turn)
        turn_right();
        delay_ms(800); 

        // 8. STOP (300ms)
        move_stop();
        delay_ms(300); 
    }
}
