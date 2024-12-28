/**
 * @ Author: Élodie Beauchemin
 * @ Create Time: 2024-12-04 15:50:56
 * @ Modified by: Élodie Beauchemin
 * @ Modified time: 2024-12-27 19:23:07
 * @ Description: Main.cpp
 */

//Libraries
#include <Arduino.h>
#include "inch2cent.h"

//Pins declaration
int keyboardPin = A5;
int delIncertitude = 16;

int dig1 = 5;
int dig2 = 7;
int dig3 = 9;
int A = 27;
int B = 14;
int C = 6;
int D = 8;
int E = 4;
int F = 26;
int G = 15;
int DP = 10;

//Global variable
String number = "";
size_t numberLength = 0;

void setup() {
  // Display - Pin configuration
  pinMode(dig1, OUTPUT);
  pinMode(dig2, OUTPUT);
  pinMode(dig3, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DP, OUTPUT);
  pinMode(delIncertitude, OUTPUT);
  pinMode(keyboardPin, INPUT);

  //Setting AREF at 3V
  analogReference(EXTERNAL); 

  //Display - Disable all digits
  clearDigits();
}

void loop() {
  program();
}