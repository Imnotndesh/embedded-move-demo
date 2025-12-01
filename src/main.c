#include <stdio.h> 

__sfr __at(0x88) TCON;
__sfr __at(0x89) TMOD;
__sfr __at(0x8d) TH1;
__sfr __at(0x98) SCON;
__sfr __at(0x99) SBUF;

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
    init_uart_9600();

    while (1)
    {
        printf("Hello There :)\n");
        delay_ms(5000); 
    }
}