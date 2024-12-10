#ifndef DO_ADC_H
#define DO_ADC_H

#include <xc.h>

// Function to initialize the ADC
void ADCinit();

// Function to perform an ADC conversion, returns result
uint16_t do_ADC(void);

#endif