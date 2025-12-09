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


void main(void)
{
    P2M0 &= ~0x0F; 
    P2M1 &= ~0x0F; 

    init_uart_9600();
    
    printf("--- Robot Control Ready --- \n");
    delay_ms(100); 

    while (1)
    {
        // Should really play with the timing since i could not figure out pwm
        move_forward();
        delay_ms(300); 

        move_back();
        delay_ms(300); 

        move_left();
        delay_ms(250); 

        move_back();
        delay_ms(300); 
        
        move_back();
        delay_ms(300); 
        
        move_right();
        delay_ms(250); 

        move_back();
        delay_ms(300); 
    }
}