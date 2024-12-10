#include <xc.h>
#include "IO.h"
#include "macros.h"

extern uint8_t PB1_old_state;
extern uint8_t PB2_old_state;
extern uint8_t PB3_old_state;

extern uint8_t PB1_press_flag;
extern uint8_t PB1_release_flag;
extern uint8_t PB2_press_flag;
extern uint8_t PB2_release_flag;
extern uint8_t PB3_press_flag;
extern uint8_t PB3_release_flag;

void IOinit()
{
    // Setup IO Ports
    
    AD1PCFG = 0xFFFF; // Set analog/digital pins as digital only
    AD1PCFGbits.PCFG5 = 0;
    // Leave AN5 (RA3) as analog
    
    TRISBbits.TRISB8 = 0; // Set RB8 as an output (LED)
    
    TRISAbits.TRISA4 = 1; // Set RA4 as an input (PB3)
    CNPU1bits.CN0PUE = 1; // Enable Pull Up Resistor (set value normally high)
    CNEN1bits.CN0IE = 1; // Enable CN interrupt for RA4
    
    TRISBbits.TRISB4 = 1; // Set RB4 as an input (PB2)
    CNPU1bits.CN1PUE = 1; // Enable Pull Up Resistor (set value normally high)
    CNEN1bits.CN1IE = 1; // Enable CN interrupt for RB4

    TRISAbits.TRISA2 = 1; // Set RA2 as an input (PB1)
    CNPU2bits.CN30PUE = 1; // Enable Pull Up Resistor (set value normally high)
    CNEN2bits.CN30IE = 1; // Enable CN interrupt for RA2
    
    // Enable CN Interrupts
    IPC4bits.CNIP = 6; // CN Interrupt priority is 6
    IFS1bits.CNIF = 0; // Set interrupt flag to 0
    IEC1bits.CNIE = 1; // Enable CN interrupts
}

void IOcheck()
{
    // Figure out current state of PBs
    uint8_t PB1_state = PB1_PRESSED;
    uint8_t PB2_state = PB2_PRESSED;
    uint8_t PB3_state = PB3_PRESSED;
    
    // Set the appropriate press/release flags
    if (PB1_state && !PB1_old_state)
        PB1_press_flag = 1;
    else if (!PB1_state && PB1_old_state)
        PB1_release_flag = 1;
    
    if (PB2_state && !PB2_old_state)
        PB2_press_flag = 1;
    else if (!PB2_state && PB2_old_state)
        PB2_release_flag = 1;
    
    if (PB3_state && !PB3_old_state)
        PB3_press_flag = 1;
    else if (!PB3_state && PB3_old_state)
        PB3_release_flag = 1;
    
    // Update PB old states
    PB1_old_state = PB1_state;
    PB2_old_state = PB2_state;
    PB3_old_state = PB3_state;
}