/*
 * File:   main.c
 * Author: Marcus Skarsgard, Kassem Jarrah, Yassen Teamma
 *
 * Created on: October 1st, 2024
 */

#include "ISR_defines.h"
#include "macros.h"

extern uint8_t T1_event;
extern uint8_t T3_event;
extern uint8_t CN_event;

extern uint8_t TimerDoneFlag;

extern uint8_t timer_minutes;
extern uint8_t timer_seconds;
extern uint8_t Program_State;


// Timer 1 interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    static uint8_t count = 0;
    
    if (Program_State == COUNTDOWN_STATE) {
        
        if (count == 1) {
            
            if (timer_seconds == 0) {  // Check if seconds are already at 0
                
                if (timer_minutes > 0) {
                    timer_minutes--;    // Decrement minutes
                    timer_seconds = 59; // Reset seconds to 59
                } 
                else 
                    TimerDoneFlag = 1;  // Set flag when countdown ends
            } 
            else 
                timer_seconds--;        // Decrement seconds if not 0
            count = 0;
        } 
        else 
            count = 1;
    }
    
    // Set global interrupt event flag
    T1_event = 1;
    
    // Don't forget to clear the timer 1 interrupt flag!
    IFS0bits.T1IF = 0;
}

// Timer 2 interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{
    
    //Don't forget to clear the timer 2 interrupt flag!
    IFS0bits.T2IF = 0;
}

// Timer 3 interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
{
    T3_event = 1;
    
    //Don't forget to clear the timer 3 interrupt flag!
    IFS0bits.T3IF = 0;
}

// Change Notification interrupt subroutine
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    CN_event = 1;
    
    //Don't forget to clear the CN interrupt flag!
    IFS1bits.CNIF = 0;
}
