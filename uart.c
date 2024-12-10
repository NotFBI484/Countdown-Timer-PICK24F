/*
 * File:   uart.c
 * Author: psbta
 * Modified by: Marcus Skarsgard
 *
 * Created on October 10, 2023, 4:10 PM
 */

#include "xc.h"
#include "uart.h"
#include "string.h"
#include "macros.h"

uint8_t received_char = 0;
extern uint8_t RXFlag;

void InitUART2(void) 
{
	// configures UART2 module on pins RB0 (Tx) and RB1 (Rx) on PIC24F16KA101 
	// Enables UART2 

	U2MODEbits.USIDL = 0;	// Bit13 Continue in Idle
	U2MODEbits.IREN = 0;	// Bit12 No IR translation
	U2MODEbits.RTSMD = 0;	// Bit11 Flow Control Mode Mode
	U2MODEbits.UEN = 00;		// Bits8,9 TX,RX enabled, CTS,RTS not
	U2MODEbits.WAKE = 0;	// Bit7 No Wake up (since we don't sleep here)
	U2MODEbits.LPBACK = 0;	// Bit6 No Loop Back
	U2MODEbits.ABAUD = 0;	// Bit5 No Autobaud (would require sending '55')
	U2MODEbits.RXINV = 0;	// Bit4 Idle state is '1'
	U2MODEbits.BRGH = 1;	// Bit3 4 clocks per bit period
	U2MODEbits.PDSEL = 0;	// Bits1,2 8bit, No Parity
	U2MODEbits.STSEL = 0;	// Bit0 One Stop Bit
    
    if (OSCCONbits.COSC == 0b110)
	{
		U2BRG = 8;	// TARGET: 7000 baud @ 500 kHz FOSC (actual rate is 6944)
	}
	else if (OSCCONbits.COSC == 0b101)
	{
		U2BRG = 12;	// TARGET: 300 baud @ 32 kHz FOSC (actual rate is 300.5)
	}
	else if (OSCCONbits.COSC == 0b000)
	{
		U2BRG = 103;	// TARGET: 9600 baud @ 8 MHz FOSC (actual rate is 9615.4)
	}
 

    U2STAbits.UTXISEL1 = 0;	//Bit15 Int when Char is transferred (1/2 config!)
    U2STAbits.UTXISEL0 = 1;	//Generate interrupt with last character shifted out of U2TXREG buffer
	U2STAbits.UTXINV = 0;	//Bit14 N/A, IRDA config
	U2STAbits.UTXBRK = 0;	//Bit11 Disabled
	U2STAbits.UTXEN = 0;	//Bit10 TX pins controlled by periph
//	U2STAbits.UTXBF = 0;	//Bit9 *Read Only Bit*
//	U2STAbits.TRMT = 0;		//Bit8 *Read Only bit*
	U2STAbits.URXISEL = 0;	//Bits6,7 Int. on character recieved
	U2STAbits.ADDEN = 0;	//Bit5 Address Detect Disabled
//	U2STAbits.RIDLE = 0;	//Bit4 *Read Only Bit*
//	U2STAbits.PERR = 0;		//Bit3 *Read Only Bit*
//	U2STAbits.FERR = 0;		//Bit2 *Read Only Bit*
//	U2STAbits.OERR = 0;		//Bit1 *Read Only Bit*
//	U2STAbits.URXDA = 0;	//Bit0 *Read Only Bit*

	
    IFS1bits.U2TXIF = 0;	// Clear the Transmit Interrupt Flag
    IPC7bits.U2TXIP = 3; // UART2 TX interrupt has interrupt priority 3-4th highest priority
    
	IEC1bits.U2TXIE = 1;	// Enable Transmit Interrupts
	IFS1bits.U2RXIF = 0;	// Clear the Recieve Interrupt Flag
	IPC7bits.U2RXIP = 4;    //UART2 Rx interrupt has 2nd highest priority
    IEC1bits.U2RXIE = 1;	// Enable Recieve Interrupts

	U2MODEbits.UARTEN = 1;	// And turn the peripheral on
    
//	U2STAbits.UTXEN = 1;
}

void Disp2String(char *str) //Displays String of characters
{
    unsigned int i;
    for (i=0; i<= strlen(str); i++)
    {
        XmitUART2(str[i],1);
    }

    return;
}

void XmitUART2(char CharNum, unsigned int repeatNo)
{	
	U2STAbits.UTXEN = 1;
	while(repeatNo!=0) 
	{
		while(U2STAbits.UTXBF==1)   // Wait while the transmit buffer is full
		{
		}	
		U2TXREG=CharNum;            // CharNum is the character we are transmitting,
                                    // we put it into this register to be transmitted 
                                    // as this is the TX register
		repeatNo--;
	}
	while(U2STAbits.TRMT==0)        // Wait while the transmit register is not empty
	{                               
	}

    U2STAbits.UTXEN = 0;
}

char RxUartCharNum()
{    
    // Return '0' to '9' or enter key
    if ((received_char >= '0') && (received_char <= '9'))
        return received_char; // 48 - 57, char ascii code
    
    else if (received_char == 0x0D)
        return 1; // enter key code
    
    else if(received_char == 'i')
        return 2; // 'i' character code
    
    else
        return 0; // bad character code
}

// Displays 16 bit unsigned in in decimal form
void Disp2Dec(uint16_t Dec_num) {
    uint8_t rem; //remainder in div by 10
    uint16_t quot;
    uint8_t ctr = 0; //counter
    //XmitUART2(' ',1); // Disp Gap
    
    while(ctr < 5) {
        quot = Dec_num/(pow(10,(4-ctr)));
        rem = quot%10;
        XmitUART2(rem + 0x30 , 1);
        ctr = ctr + 1;
    }
    
    //XmitUART2(' ',1); // Disp Gap
    return;
}

// Function to display an 8 bit number on uart
void UartDisplayDec(uint8_t number){
    
    // Each element in this array represents a digit (of 3 possible)
    uint8_t units[3];
    units[0] = number / 100; // extract hundreds
    units[1] = (number % 100) / 10; // extract tens
    units[2] = number % 10; // extract ones
    
    
    for (uint8_t i = 0; i < 3; i++)
    {
        if ((i == 0) && (units[0] != 0))
            XmitUART2((char) units[0] + '0', 1);
        else if (i != 0)
            XmitUART2((char) units[i] + '0', 1);
    }
}

void __attribute__ ((interrupt, no_auto_psv)) _U2RXInterrupt(void) {
    
	IFS1bits.U2RXIF = 0;
    
    // Grab the most recently entered character
    received_char = U2RXREG;   

    RXFlag = 1;
}

void __attribute__ ((interrupt, no_auto_psv)) _U2TXInterrupt(void) {
	IFS1bits.U2TXIF = 0;

}
