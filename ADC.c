/*
 * File:   main.c
 * Author: Marcus Skarsgard, Kassem Jarrah, Yassen Teaama
 *
 * Created on October 29th, 2024, 8:30 AM
 */

#include "ADC.h"

void ADCinit()
{
    // ADC1CON1 config
    AD1CON1bits.ADON = 0; // Turn ADC on
    AD1CON1bits.ADSIDL = 1; // Turn off ADC in Idle mode
    AD1CON1bits.FORM = 0; // Set output form as 0000 00dd dddd dddd
    AD1CON1bits.SSRC = 7; // Auto convert
    AD1CON1bits.ASAM = 0; // Sampling begins when SAMP bit is set
    AD1CON1bits.DONE = 0; // Clear sampling conversion completion bit
    
    // AD1CON2 config
    AD1CON2bits.VCFG = 0; // Set voltage reference to VDD and GND
    AD1CON2bits.CSCNA = 0; // Don't scan inputs
    AD1CON2bits.BUFM = 0; // Use 16 bit buffer as a single buffer
    AD1CON2bits.ALTS = 0; // Always use MUX A
    
    // AD1CON3 config
    AD1CON3bits.ADRC = 0; // Use system clock
    AD1CON3bits.SAMC = 0x03; // Use 3 TAD for sampling time
    AD1CON3bits.ADCS = 0x02; // Use 4 * TCY for conversion time 
    
    // AD1CHS config
    AD1CHSbits.CH0NA = 0; // Use MCU GND as ADC negative reference
    AD1CHSbits.CH0SA = 5; // Use AN5 as ADC positive reference
    
    AD1CSSLbits.CSSL5 = 0; // Take AN5 (RA3) off the scan list
}

uint16_t do_ADC(void) 
{
    uint16_t ADCvalue = 0; // This will hold the measured ADC value to be returned
    
    AD1CON1bits.ADON = 1; //  Turn on the ADC
    
    AD1CON1bits.SAMP = 1; // Start sampling
    
    while(AD1CON1bits.DONE == 0); // Busy wait while the conversion is happening
    
    ADCvalue = ADC1BUF0; // Set the ADCvalue with the converted voltage level
    
    AD1CON1bits.SAMP = 0; // Stop sampling
    
    AD1CON1bits.ADON = 0; //  Turn off the ADC
    
    return ADCvalue; // Return the measured ADC value
}
