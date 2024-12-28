/**
 * @ Author: Élodie Beauchemin
 * @ Create Time: 2024-12-07 14:35:19
 * @ Modified by: Élodie Beauchemin
 * @ Modified time: 2024-12-27 19:22:55
 * @ Description: Header file for the Inch2Cent projet
 */

#ifndef INCH2CENT_H_INCLUDED
#define INCH2CENT_H_INCLUDED

//Librairies
#include <Arduino.h>

//Global variables
extern int keyboardPin;
extern int delIncertitude;
extern int dig1, dig2, dig3, A, B, C, D, E, F, G, DP;
extern String number;
extern size_t numberLength;

//Display patterns
//{A, B, C, D, E, F, G}
const uint8_t defaultPattern[7] = {0, 0, 0, 0, 0, 0, 1}; // - (middle line)
const uint8_t segmentPatterns[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
};

//Functions declaration
int buttonSelected();
bool addNumber(int buttonS);
float inch2cent(float numToConvert);
void setNumber(int number, bool isDP);
void setDigit(int digit, int number, bool isDP);
int setDisplay(float number);
float roundNumber(float value, int precision);
void clearDigits();
void program();

void test();

#endif // INCH2CENT_H_INCLUDED