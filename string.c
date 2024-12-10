/*
 * File:   main.c
 * Author: Marcus Skarsgard, Kassem Jarrah, Yassen Teaama
 *
 * Created on November 12th, 2024, 4:36 PM
 */

#include <xc.h>

void RstString(char *String)
{
    // Reset timer string to {'0', '0', ':', '0', '0', '\0'}
    String[0] = '0';
    String[1] = '0';
    String[2] = ':';
    String[3] = '0';
    String[4] = '0';
    String[5] = '\0';
}

void StringToTime(char *String, uint8_t *mins, uint8_t *secs)
{
    // Temporary variables to hold time in minutes and seconds
    uint8_t tempMins = 0;
    uint8_t tempSecs = 0;
    
    // Calculate the raw number of minutes, and raw number of seconds, subtracting off '0' to convert ascii to decimal
    tempMins = (String[0] - '0') * 10 + (String[1] - '0');
    tempSecs = (String[3] - '0') * 10 + (String[4] - '0');
    
    // Put the values into the in pointers
    *mins = tempMins + (tempSecs / 60);
    *secs = tempSecs % 60;
}