#ifndef ISR_DEFINES_H
#define ISR_DEFINES_H

#include <xc.h> 

// Timer 1 interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void);

// Timer 2 interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);

// Timer 3 interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void);

// Change Notification interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void);

#endif