#ifndef MACROS_H
#define MACROS_H

#define T1_INT_STATE IEC0bits.T1IE
#define T2_INT_STATE IEC0bits.T2IE
#define T3_INT_STATE IEC0bits.T3IE
#define CN_INT_STATE IEC1bits.CNIE

#define T1_STATE T1CONbits.TON
#define T2_STATE T2CONbits.TON
#define T3_STATE T3CONbits.TON
    
#define PB1_PRESSED PORTAbits.RA2 == 0 // True if PB1 is pressed
#define PB2_PRESSED PORTBbits.RB4 == 0 // True if PB2 is pressed
#define PB3_PRESSED PORTAbits.RA4 == 0 // True if PB3 is pressed

#define LED_ENABLE OC1CONbits.OCM = 5
#define LED_DISABLE OC1CONbits.OCM = 0

#define WAITING_STATE 0
#define ENTRY_STATE 1
#define COUNTDOWN_STATE 2
#define PAUSED_STATE 3
#define COMPLETE_STATE 4

#endif