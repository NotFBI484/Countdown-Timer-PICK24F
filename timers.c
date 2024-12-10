/*
 * File:   main.c
 * Author: Marcus Skarsgard, Kassem Jarrah, Yassen Teaama
 *
 * Created on November 12th, 2024, 12:16 PM
 */

#include <xc.h>
#include "timers.h"

void T1init()
{
    T1CONbits.TCKPS = 1; // set prescaler to 1:256
    T1CONbits.TCS = 0; // use internal clock    
    T1CONbits.TSIDL = 0; //operate in idle mode
    IPC0bits.T1IP = 2; //7 is highest and 1 is lowest priority.
    IFS0bits.T1IF = 0; // Clear Timer 1 interrupt flag
    IEC0bits.T1IE = 1; //enable timer 1 interrupt
    PR1 = 15625; // set the timer 1 count value for 250ms
    TMR1 = 0; // Start timer 2 at 0
    
    T1CONbits.TON = 1;
}

void T2init()
{
    T2CONbits.T32 = 0; // operate timer 2 as 16 bit timer
    T2CONbits.TCKPS = 3; // set prescaler to 1:256
    T2CONbits.TCS = 0; // use internal clock    
    T2CONbits.TSIDL = 0; //operate in idle mode
    IPC1bits.T2IP = 2; //7 is highest and 1 is lowest priority.
    IFS0bits.T2IF = 0; // Clear Timer 2 interrupt flag
    IEC0bits.T2IE = 1; //enable timer 2 interrupt
    
    T2CONbits.TON = 0; // Leave timer off for now
}

void T3init()
{
    T3CONbits.TCKPS = 0;  // 1:1 prescaler
    T3CONbits.TCS = 0;    // internal clock 
    T3CONbits.TSIDL = 0;  // idle mode operation
    PR3 = 249;              // count to 249   
    TMR3 = 0;             // start timer 3 at 0
    IPC2bits.T3IP = 3;    // priority    
    IFS0bits.T3IF = 0;    // clear timer 3 interrupt flag   
    IEC0bits.T3IE = 1;    // enable timer 3 interrupt
    
    T3CONbits.TON = 1;    // Turn on timer 3
}