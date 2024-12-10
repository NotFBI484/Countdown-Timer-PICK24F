/*
 * File:   main.c
 * Author: Marcus Skarsgard, Kassem Jarrah, Yassen Teaama
 *
 * Created on November 12th, 2024, 12:16 PM
 */

#include "pragma_config.h"
#include <xc.h>
#include <p24F16KA101.h>
#include "macros.h"
#include "clkChange.h"
#include "uart.h"
#include "timers.h"
#include "IO.h"
#include "state_defines.h"
#include "delay_ms.h"
#include "ADC.h"
#include "ISR_defines.h"
#include "pwm.h"
#include "string.h"
#include "uart_escape_codes.h"

// Event flags
uint8_t CN_event = 0;
uint8_t T1_event = 0;
uint8_t T3_event = 0;
uint8_t RXFlag = 0;
uint8_t TimerDoneFlag = 0;

uint8_t PB1_press_flag = 0;
uint8_t PB1_release_flag = 0;
uint8_t PB2_press_flag = 0;
uint8_t PB2_release_flag = 0;
uint8_t PB3_press_flag = 0;
uint8_t PB3_release_flag = 0;

// Variables to hold previous button states for rising edge detection 
uint8_t PB1_old_state = 0;
uint8_t PB2_old_state = 0;
uint8_t PB3_old_state = 0;

// Set the initial program state
uint8_t Program_State = WAITING_STATE;

// Global variables that hold the current timer times
uint8_t timer_minutes = 0;
uint8_t timer_seconds = 0;

// Initialize the UART timer string buffer 
char timer_string[6] = {'0', '0', ':', '0', '0', '\0'};

uint16_t ADC_value = 1023;

int main(void) {
    
    // Initialize IO Ports
    IOinit();
    
    // Initialize the clock at 500khz
    newClk(500);
    
    // Initialize UART2
    InitUART2(); // NOTE THAT UART2 RUNS at 7000 BAUD
    
    // Timer 1 initialization
    T1init();
    
    // Timer 2 initialization
    T2init();
    
    // Timer 3 initialization
    T3init();
    
    // Initialize the PWM peripheral
    PWMinit();
    
    // ADC initialization
    ADCinit();
    
    // Turn T1 interrupts off, T2 interrupts off, T3 interrupts on and CN interrupts on
    T1_INT_STATE = 1;
    T2_INT_STATE = 0;
    T3_INT_STATE = 1;
    CN_INT_STATE = 1;
    
    // Reset terminal screen
    Disp2String(reset_screen);
    
    // Waiting state entry code
    Disp2String("Press PB1 to enter timer value");

    while(1)
    {    
        Idle();
        
        // All states use push button IO, so the function
        // call is placed here as this portion of the loop is inherent to all states
        if(CN_event)
        {
            delay_ms(30);
            IOcheck();
            CN_event = 0;
        }
        
        ADC_value = do_ADC();

        switch(Program_State)
        {
            case(WAITING_STATE):
                Waiting_State();
                break;
                
            case(ENTRY_STATE):
                Entry_State();
                break;
                
            case(COUNTDOWN_STATE):
                Countdown_State();
                break;
            
            case(PAUSED_STATE):
                Paused_State();
                break;
                
            case(COMPLETE_STATE):
                Complete_State();
                break;
                
            default:
                Error_State();
                break;
        }
    } 

    return 0;
}
