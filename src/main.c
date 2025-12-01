#include <stdio.h> 
#include <stdbool.h>
// Errors are there but linter ndo mrazi ngl since it compiles
__sfr __at(0x88) TCON;
__sfr __at(0x89) TMOD;
__sfr __at(0x8d) TH1;
__sfr __at(0x98) SCON;
__sfr __at(0x99) SBUF;

__sfr __at(0xA2) P2M0; 
__sfr __at(0xA3) P2M1; 

__sbit __at(0xA0 + 0) S1_PIN;
__sbit __at(0xA0 + 1) S2_PIN;
__sbit __at(0xA0 + 2) S3_PIN;
__sbit __at(0xA0 + 3) S4_PIN;


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

void main(void)
{
    P2M0 = 0x0F; 
    P2M1 = 0x0F; 

    init_uart_9600();
    
    printf("Robot Control Initialized. Waiting for button press...\n");
    delay_ms(100); 

    while (1)
    {
        if (S1_PIN == 0) { 
            printf("S1 PRESSED: MOVE FORWARD\n"); 
            while (S1_PIN == 0);
            delay_ms(100); 
        }

        if (S2_PIN == 0) { 
            printf("S2 PRESSED: MOVE BACKWARD\n"); 
            while (S2_PIN == 0);
            delay_ms(100);
        }

        if (S3_PIN == 0) { 
            printf("S3 PRESSED: TURN LEFT\n"); 
            while (S3_PIN == 0); 
            delay_ms(100); 
        }

        if (S4_PIN == 0) { 
            printf("S4 PRESSED: TURN RIGHT\n"); 
            while (S4_PIN == 0); 
            delay_ms(100); 
        }

        delay_ms(10);
    }
}