#ifndef PWM_H
#define PWM_H

#include <xc.h>

// Initialization function for the PWM peripheral
void PWMinit();

// Function used to modulate brightness during the LED pulsing stage
void PWMcheck();

// Function used to set the brightness of the LED based on the adc reading
void PWMled(uint16_t adc_value);
/* REQUIRES:
 *      - adc_value contains an integer ranging from 0 - 1023 (10 bits)
 *      - the LED is connected to pin RA6
 * 
 * PROMISES:
 *      - Sets the brightness of the LED using PWM. 
 *      - LED brightness is directly proportional to adc_value
 */

#endif // PWM_H