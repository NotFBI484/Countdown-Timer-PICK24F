#ifndef UART_ESCAPE_CODES_H
#define UART_ESCAPE_CODES_H

// When sent, erases everything to the right of the cursor
char erase_right[] = {0x1B, '[', 'K', '\0'};

// When sent, moves cursor to the first column of the first line (home)
char go_home[] = {0x1B, '[', 'H', '\0'};

// Clears the screen and moves cursur to home 
char reset_screen[] = {0x1B, '[', '2', 'J', 0x1B, '[', 'H', '\0'};

// Save cursor position
char savepos[] = {0x1B, '7', '\0'};

// Load saved cursor position
char loadpos[] = {0x1B, '8', '\0'};

#endif // UART_ESCAPE_CODES_H