#include "state_defines.h"
#include "xc.h"
#include "macros.h"
#include "uart.h"
#include "pwm.h"
#include "string.h"

// Bring in the global program state variable
extern uint8_t Program_State;

// Bring in global event flags
extern uint8_t T1_event;
extern uint8_t T3_event;
extern uint8_t RXFlag;
extern uint8_t TimerDoneFlag;

// Bring in global PB state flags
extern uint8_t PB1_press_flag;
extern uint8_t PB1_release_flag;
extern uint8_t PB2_press_flag;
extern uint8_t PB2_release_flag;
extern uint8_t PB3_press_flag;
extern uint8_t PB3_release_flag;

// Bring in some necessary ascii escape sequences
extern char erase_right[];
extern char go_home[];
extern char savepos[];
extern char loadpos[];
extern char reset_screen[];

// global values of minutes and seconds
extern uint8_t timer_minutes;
extern uint8_t timer_seconds;

extern char timer_string[];

extern uint16_t ADC_value;

void Waiting_State(void){
    
    // Handle LED pulsing
    if (T3_event)
    {
        PWMcheck();
        T3_event = 0;
    }
    
    // Handle T1 interrupt flags (not used in this state)
    if (T1_event)
        T1_event = 0;
    
    // Ignore any keyboard presses in this state
    if (RXFlag)
        RXFlag = 0;
    
    // Waiting State exit code -> entry state
    if (PB1_press_flag)
    {
        // Clear button flags
        PB1_press_flag = 0; 
        PB1_release_flag = 0;
        
        Program_State = ENTRY_STATE; // Change program state
        
        // Print off Entry state information
        Disp2String(go_home);
        Disp2String(erase_right);
        Disp2String("Timer: ");
        
        // Save the cursor position
        Disp2String(savepos);
        
        // Print off the current timer string ("00:00")
        Disp2String(timer_string);
        
        // Go back to the saved position
        Disp2String(loadpos);
    }
    
    // Ignore any state changes to these flags
    PB2_press_flag = 0;
    PB2_release_flag = 0;
    PB3_press_flag = 0;
    PB3_release_flag = 0;
}

void Entry_State(void){
    
    static uint8_t index = 0;
    static uint8_t ready = 0;
    
    // Handle LED pulsing
    if (T3_event)
    {
        PWMcheck();
        T3_event = 0;
    }
    
    // handle T1 interrupt flags (not used in this state)
    if (T1_event)
        T1_event = 0;
    
    if (RXFlag){
        RXFlag = 0;
        
        // Fetch the most recently received character
        char Rxd_Char = RxUartCharNum();
        
        // Only add the character to the timer if RxUartCharNum didn't return an error code
        if (Rxd_Char > 2)
        {
            
            // Roll index over if string length exceeded
            if (index == 5)
            {
                index = 0;
                Disp2String(loadpos);
            }
            // Skip over the ':' character
            else if (index == 2){
                index++;
                XmitUART2(':', 1);
            }
                
            // put received character into the timer string
            timer_string[index] = Rxd_Char;
            
            // increment index
            index++;
            
            XmitUART2(Rxd_Char, 1);
        }
        
        // If the received char is an enter key, go to ready mode
        if (Rxd_Char == 1)
        {
            // Display entered time, set ready flag
            Disp2String(go_home);
            Disp2String(erase_right);
            Disp2String("Entered time: ");
            Disp2String(timer_string);
            ready = 1;
        }
        
    }
    
    // Reset timer value and print off data if PB2 clicked
    if (PB2_press_flag && PB2_release_flag)
    {
        // Clear button flags
        PB2_press_flag = 0;
        PB2_release_flag = 0;
        
        // Reset the entered timer value
        RstString(timer_string);
        index = 0;
        ready = 0;
        
        // Print off timer info
        Disp2String(go_home);
        Disp2String(erase_right);
        Disp2String("Timer: ");
        
        // Save the cursor position
        Disp2String(savepos);
        
        // Print off the current timer string ("00:00")
        Disp2String(timer_string);
        
        // Go back to the saved position
        Disp2String(loadpos);
    }
    
    // Entry State exit code -> waiting state
    if (PB1_press_flag)
    {
        // Clear button flags
        PB1_press_flag = 0; 
        PB1_release_flag = 0; 
        
        // Reset the entered timer value
        RstString(timer_string);
        index = 0;
        ready = 0;
        
        Program_State = WAITING_STATE; // Change program state
        
        // Print off waiting state information
        Disp2String(go_home);
        Disp2String(erase_right);
        Disp2String("Press PB1 to enter timer value");
    }
    
    // Entry state exit code -> countdown state
    if (PB3_press_flag && PB3_release_flag)
    {
        if (ready)
        {
            // Clear button flags
            PB3_press_flag = 0; 
            PB3_release_flag = 0; 
            ready = 0;

            // Convert the timer_string to minutes and seconds
            StringToTime(timer_string, &timer_minutes, &timer_seconds);
            
            // Now it's safe to reset the key variables for this state
            RstString(timer_string);
            index = 0;
            ready = 0;
            
            Program_State = COUNTDOWN_STATE;
            Disp2String(go_home);
            Disp2String(erase_right);
            
            UartDisplayDec(timer_minutes);
            XmitUART2(':', 1);
            UartDisplayDec(timer_seconds);
        }
        else
        {
            // Clear button flags
            PB3_press_flag = 0; 
            PB3_release_flag = 0; 
        }
        
    }
    
}

void Countdown_State(void){
    
    // Holds current led mode (1 if on, 0 if off)
    static uint8_t led_mode = 0;
    
    static uint8_t PB3_hold_duration = 0;
    
    // 0 for basic, 1 for extra info
    static uint8_t display_mode = 0;
    
    static uint8_t new_info_flag = 0;
    
    static uint16_t old_ADC = 0;
    
    if ((old_ADC - ADC_value > 5) || (old_ADC - ADC_value < -5))
        new_info_flag = 1;
    else
        new_info_flag = 0;
    
    old_ADC = ADC_value;
        
    
    // handle any T3 interrupt flags (not used in this state)
    if (T3_event)
        T3_event = 0;
    
    if (RXFlag){
        RXFlag = 0;
        
        char Rxd_Char = RxUartCharNum();
        
        if (Rxd_Char == 2){
            display_mode ^= 1;
        }
    }
    
    // Handle displaying data every second, as well as toggling LED
    if (T1_event){
        T1_event = 0;
        
        if (led_mode)
        {
            LED_ENABLE;
            PWMled(ADC_value);
            led_mode = 0;
            
            Disp2String(go_home);
            Disp2String(erase_right);
            UartDisplayDec(timer_minutes);
            XmitUART2(':', 1);
            UartDisplayDec(timer_seconds);
            
            if (display_mode && new_info_flag){
                XmitUART2('\n',1);
                XmitUART2('\r',1);
                Disp2String(" ADC: ");
                Disp2Dec(ADC_value);
                Disp2String(" LED: ");
                UartDisplayDec((uint16_t) ((uint32_t) ADC_value * 100 / 1023));
                XmitUART2('%',1);
                Disp2String(erase_right);
            }
            
            if(!display_mode){
                XmitUART2('\n',1);
                XmitUART2('\r',1);
                Disp2String(erase_right);
                Disp2String(go_home);
            }
                
            
            // Every second, check if PB3 is still being held, and still hasn't been release,
            // if so, increment the PB3_hold_duration counter by 1 second
            if (PB3_press_flag && !PB3_release_flag) 
                PB3_hold_duration++;
        }
        else
        {
            LED_DISABLE;
            led_mode = 1;
        }  
        
    }
    
    // Handle the transition to Complete_State when the timer is done
    if (TimerDoneFlag){
        
        TimerDoneFlag = 0;
        
        PB3_hold_duration = 0;
        
        // change state variable
        Program_State = COMPLETE_STATE;
        
        // Turn the LED ON
        PWMled(ADC_value);
        
        // print data
        Disp2String(go_home);
        Disp2String(erase_right);
        UartDisplayDec(timer_minutes);
        XmitUART2(':', 1);
        UartDisplayDec(timer_seconds);
    }
    
    // Handle transitions to complete state when PB3 held for 3 seconds
    if (PB3_hold_duration == 3){
        
        // reset button flags
        PB3_press_flag = 0;
        PB3_release_flag = 0;
        PB3_hold_duration = 0;
        
        // change state variable
        Program_State = COMPLETE_STATE;
        
        // Turn the LED ON
        PWMled(ADC_value);
        
        // display data
        Disp2String(go_home);
        Disp2String(erase_right);
        UartDisplayDec(00);
        XmitUART2(':', 1);
        UartDisplayDec(00);
    }
    
    // Handle transitions to Paused_State
    if (PB3_press_flag && PB3_release_flag){
        
        PB3_hold_duration = 0;
        
        // reset button flags
        PB3_press_flag = 0;
        PB3_release_flag = 0;
        
        // change state variable
        Program_State = PAUSED_STATE;
        
        // Turn off timer 1 to stop decrementing the time
        T1_STATE = 0;
        
        // display data
        Disp2String(go_home);
        Disp2String(erase_right);
        UartDisplayDec(timer_minutes);
        XmitUART2(':', 1);
        UartDisplayDec(timer_seconds);
    }
    
    PB1_press_flag = 0;
    PB1_release_flag = 0;
    PB2_press_flag = 0;
    PB2_release_flag = 0;
    
}

void Paused_State(void){
    
    // Handle any unused flags for this state
    if (RXFlag){
        RXFlag = 0;
    }
    if (T3_event){
        T3_event = 0;
    }
    if (T1_event){
        T1_event = 0;
    }
    
    if (PB3_press_flag && PB3_release_flag){
        // reset button flags
        PB3_press_flag = 0;
        PB3_release_flag = 0;
        
        // change state variable
        Program_State = COUNTDOWN_STATE;
        
        // Turn off timer 1 to allow decrementing of time
        T1_STATE = 1;
    }
    
    // acknowledge/clear other button flags
    PB1_press_flag = 0;
    PB1_release_flag = 0;
    PB2_press_flag = 0;
    PB2_release_flag = 0; 
        
}

void Complete_State(void){
    
    static uint8_t state_countdown = 10;
    
    PWMled(ADC_value); // test
    
    // Handle T1 interrupts
    if (T1_event){
        T1_event = 0;
        
        // decrement every half second
        state_countdown--;
        
        // Set LED to potentiometer value
    }
    
    if (state_countdown == 8){
        Disp2String(go_home);
        Disp2String(erase_right);
        Disp2String("DONE!");
    }
    
    // Handle timeout transition to waiting state
    if (state_countdown == 0){
        state_countdown = 10;
        
        Disp2String(reset_screen);
        Disp2String("Press PB1 to enter timer value");
        
        Program_State = WAITING_STATE;
    }
    
    if (PB1_press_flag || PB2_press_flag || PB3_press_flag){
        state_countdown = 10;
        
        // reset button flags
        PB1_press_flag = 0;
        PB1_release_flag = 0;
        PB2_press_flag = 0;
        PB2_release_flag = 0;
        PB3_press_flag = 0;
        PB3_release_flag = 0;
        
        Disp2String(reset_screen);
        Disp2String("Press PB1 to enter timer value");
        
        Program_State = WAITING_STATE;
        T1_STATE = 1;
    }
}

void Error_State(void){
    if (T1_event){
        Disp2String(reset_screen);
        Disp2String("Something went wrong, please reset the MCU.");
    }    
}