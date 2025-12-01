#include <stdio.h> // Required for printf

// Manual definitions of Special Function Registers (SFRs)
// This avoids relying on the complex <mcs51/8052.h> header
__sfr __at(0x88) TCON; // Timer Control
__sfr __at(0x89) TMOD; // Timer Mode
__sfr __at(0x8b) TL1;  // Timer 1 Low
__sfr __at(0x8d) TH1;  // Timer 1 High

__sfr __at(0x98) SCON; // Serial Control
__sfr __at(0x99) SBUF; // Serial Buffer

// --- Overriding putchar for printf ---
// This is the implementation from the forum post, optimized for SDCC
int putchar(int c)
{
	// Wait until TI (Transmit Interrupt, SCON bit 1, value 0x02) is set
	while(!(SCON & 0x02)); 
	SCON &= ~0x02; // Clear TI flag
	SBUF = c;
	return (c);
}

void main(void)
{
	unsigned long int i;

	// 1. Configure Timer 1 for 9600 Baud on 11.0592MHz Crystal
	TMOD = 0x20;   // Timer 1, Mode 2 (8-bit auto-reload)
	SCON = 0x50;   // Serial Mode 1 (8-bit UART), REN=1 (Receive Enable)
    
	// 2. Set reload value (0xFD = 253, calculated for 9600 baud)
	TH1 = 0xFD; 
	
	// 3. Start Timer 1
	TCON |= 0x40; // TCON bit 6 is TR1 (Timer 1 Run control)
    
	// 4. Set TI flag to 1 so the first putchar can run immediately
	SCON |= 0x02;				

	for(;;)
	{
		printf("SUCCESS: Upload works! Handshake achieved!\n");
        
        // Delay loop from the tutorial (approx 1 second)
		for(i = 0; i < 147456; i++); 
	}
}