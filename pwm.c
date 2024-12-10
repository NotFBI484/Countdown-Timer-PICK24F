/*
 * File:   main.c
 * Author: Marcus Skarsgard, Kassem Jarrah, Yassen Teaama
 *
 * Created on November 11th, 2024, 11:00 AM
 */

#include "pwm.h"
#include <xc.h>
#include "macros.h"

void PWMinit()
{
    TRISAbits.TRISA6 = 0;  // Set RA6 as output for OC1 (PWM signal)
    PR3 = 249;  // Calculated for 1kHz PWM frequency
    OC1R = 0;   // start with a 0% Duty Cycle (LED initially off)
    OC1RS = 0;  // initial duty cycle buffered
    OC1CONbits.OCTSEL = 1; // Use timer 3 as clock source for output compare
    OC1CONbits.OCM = 5;    // Edge aligned PWM mode
}

void PWMcheck()
{
    static uint16_t counter = 0;
    static uint16_t direction = 1;   // 1 for increasing brightness, -1 for decreasing
    
    counter++;
    if (counter >= 5) {  // Increment OC1RS every 16ms
        OC1RS += direction;  // Update the duty cycle
        counter = 0;  // Reset the counter

        // Reverse direction if limits are reached
        if (OC1RS >= PR3) {
            direction = -1;  // Start decreasing brightness
            OC1RS = PR3;     // Clamp to 100% duty cycle
        } else if (OC1RS <= 1) {
            direction = 1;   // Start increasing brightness
            OC1RS = 1;       // Clamp to 0% duty cycle
        }
    }
}

void PWMled(uint16_t adc_value)
{
    static uint16_t duty_cycle;   // duty_cycle value to be copied into OC1RS (for brightness adjusting)

    LED_ENABLE;
    
    if (adc_value > 1023)    // clamp the max adc_value to prevent glitching.
    {
           adc_value = 1023;
    }

    //duty_cycle = (uint32_t) (adc_value * 1023) / PR3;
    duty_cycle = adc_value >> 2;

    if (duty_cycle <= 1)
    {
        LED_DISABLE;
    }

    OC1RS = duty_cycle;
}
