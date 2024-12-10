/*
 * File:   main.c
 * Author: Marcus Skarsgard, Kassem Jarrah, Yassen Teaama
 *
 * Created on October 1st, 2024, 8:30 AM
 */

#include <xc.h>
#include "delay_ms.h"

void delay_ms(uint16_t time){
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCS = 0; // use internal clock    
    T2CONbits.TSIDL = 0; //operate in idle mode
    IPC1bits.T2IP = 2; //7 is highest and 1 is lowest priority.
    IFS0bits.T2IF = 0; // Clear Timer 2 interrupt flag
    IEC0bits.T2IE = 1; //enable timer 2 interrupt
    T2CONbits.TON = 0; // Leave timer off for now
    
    // if we're delaying for less than 16776ms we can use a more precise prescaler
    if (time < 16776)
    {
        T2CONbits.TCKPS = 2; // set prescaler to 1:64
        // This equation avoids working with floats (recall '/' is floor division in C)
        PR2 = (uint32_t) time * 3906 / 1000;
    }
    // if we're delaying for a longer time, we have to use a less precise prescaler
    else
    {
        T2CONbits.TCKPS = 3; // set prescaler to 1:256
        PR2 = time;
    }
        
    TMR2 = 0; // Start timer 2 at 0
    T2CONbits.TON = 1;
    
    Idle();
    
    T2CONbits.TON = 0;
    IEC0bits.T2IE = 0; //disable timer 2 interrupt
}