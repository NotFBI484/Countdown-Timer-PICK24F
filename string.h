#ifndef STRING_H
#define STRING_H

// Function to reset a string back to 00:00
void RstString(char *String);

// Function to convert a string of number characters to minutes and seconds
void StringToTime(char *String, uint8_t *mins, uint8_t *secs);

#endif //STRING_H