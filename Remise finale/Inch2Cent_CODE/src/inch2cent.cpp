/**
 * @ Author: Élodie Beauchemin
 * @ Create Time: 2024-12-07 14:35:30
 * @ Modified by: Élodie Beauchemin
 * @ Modified time: 2024-12-27 19:22:51
 * @ Description: Function file for the Inch2Cent projet
 */

#include "inch2cent.h"

// Determine which button is selected with threshold in function of R
int buttonSelected() {
    int buttonValue = analogRead(keyboardPin);
    int thresholds[12][2] = {
        {50, 120}, {136, 206}, {221, 291}, {306, 376},  
        {391, 461}, {477, 547}, {550, 620}, {642, 712},  
        {734, 804}, {814, 884}, {903, 973}, {981, 1023}
    };

    // Corresponding button values
    int values[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, '.', 'e'}; // Use ASCII for '.', 'e'

    // Loop through thresholds and find matching value
    for (int i = 0; i < 12; ++i) {
        if (buttonValue >= thresholds[i][0] && buttonValue <= thresholds[i][1]) {
            return values[i];
        }
    }

    return -1; // Return -1 if no button matches
}

// Adding the number selected in a string
bool addNumber(int buttonS) {
    if (buttonS != -1 && buttonS != 'e') { // Verify if a button other dans ENTRER ('e') is selected
        if (buttonS == 46) {
          number += '.';
        }
        else {
            number += String(buttonS);
        }
        numberLength++;
        digitalWrite(delIncertitude, HIGH);
        delay(500);        
    }
    
    if (buttonS == 'e') {
      Serial.println("enter is selected");
    }

    if (numberLength != number.length()){ //The number is not added in the list
        return 0;
    }

    else { // Everything is fine
        digitalWrite(delIncertitude, LOW);
        return 1;
    }
}

//Converting an length in inch to centimeters 
float inch2cent(float lengthToConvert) {
    //Adjusting the length in function of the display condition
    //Since the display is 3 digits, the length in centimeters is rounded to be shown
    if (lengthToConvert > 390) {                                //The value to display is 4 digits long or more. We don't show it.
        return 1000.;
    }
    
    //Conversion of inches in centimeters
    float centimeterLength = lengthToConvert * 2.54;

    if (lengthToConvert <= 390 && lengthToConvert > 39) {   //The value to display is 3 digits long without a decimal (the precision is lost)
        centimeterLength = roundNumber(centimeterLength, 0);
    }

    else if (lengthToConvert <= 39) {                            //The value to display is 2 digits long or less witout a decimal
        if (centimeterLength < 10) {
            centimeterLength = roundNumber(centimeterLength, 2);
        }

        else if (centimeterLength < 100) {
            centimeterLength = roundNumber(centimeterLength, 1);
        }
    }
    return centimeterLength;
}

//Alimenting the right DEL to display the digit wanted
void setNumber(int number, bool isDP) {
    if (number < 0 || number > 9) number = -1; //Default/error handling
    const uint8_t* pattern = (number == -1) ? defaultPattern : segmentPatterns[number]; //Selecting the display pattern

    //Set segments accordingly to pattern
    digitalWrite(A, pattern[0]);
    digitalWrite(B, pattern[1]);
    digitalWrite(C, pattern[2]);
    digitalWrite(D, pattern[3]);
    digitalWrite(E, pattern[4]);
    digitalWrite(F, pattern[5]);
    digitalWrite(G, pattern[6]);
  
    if (isDP) {
        digitalWrite(DP, HIGH);
    }
    else {
        digitalWrite(DP, LOW);
    }
}

//Alimenting the correct digit to light up the correct number
void setDigit(int digit, int number, bool isDP) {
    // Turn off all digits first
    clearDigits();

    switch(digit)
    {
        case 0:
            digitalWrite(dig1, LOW);
            digitalWrite(dig2, HIGH);
            digitalWrite(dig3, HIGH);
            setNumber(number, isDP);
        break;

        case 1:
            digitalWrite(dig1, HIGH);
            digitalWrite(dig2, LOW);
            digitalWrite(dig3, HIGH);
            setNumber(number, isDP);
        break;

        case 2:
            digitalWrite(dig1, HIGH);
            digitalWrite(dig2, HIGH);
            digitalWrite(dig3, LOW);
            setNumber(number, isDP);
        break;
    }
}

//Configurating the display to show the 3 digits number
//Condition : Doesn't take negative values
int setDisplay(float number) {
    int digits[3];
    int decimalPosition = 0;

    if (number == 1000.) {                 //To show --- if the value is out of range
        digits[0] = 10;
        digits[1] = 10;
        digits[2] = 10; //Declaring 10 for it to be outside of the range 0-9 and show the error display
    }

    else {                                 //To show the result
        //Converting number in function of the necessery scale
        int scaledValue;


        if (number < 10) {
            scaledValue = number * 100;     //ex : 1.25 => 125
            decimalPosition = 1;
        }

        else if (number < 100) {
            scaledValue = number * 10;      //ex : 13.2 => 132
            decimalPosition = 2;
        }

        else {
            scaledValue = number;           //ex : 100. => 100
            decimalPosition = 3;
        }

        //Getting the shown digits
        digits[0] = (scaledValue / 100) % 10;   // Hundreds
        digits[1] = (scaledValue / 10) % 10;    // Tens
        digits[2] = scaledValue % 10;           // Units
    }

    //Display the digits
   for (int i = 0; i < 3; i++) {
        clearDigits();
        delay(3);
        setDigit(i, digits[i], (i == decimalPosition - 1));
        delay(1);
   }    

    return decimalPosition;
}

//Rounding the float number with the precision asked 
//Condition : precision >= 0
float roundNumber(float value, int precision) {
    float multiplier = pow(10, precision);
    float roundedValue = (int)(value * multiplier + 0.5) / multiplier;
    return roundedValue;
}

//Reset the display (nothing is shown)
void clearDigits() {
    digitalWrite(dig1, HIGH);
    digitalWrite(dig2, HIGH);
    digitalWrite(dig3, HIGH);
}

//The entier sequence to make the product work
void program() {
    float inchLength;
    float centimeterLength;
    int decimalPosition;
    unsigned long presentTime;
    int button = -1; //Reset the value to be different from the ones on the keyboard

    button = buttonSelected(); //Reading the value pressed on the keyboard 

    if (addNumber(button) == false) //Adding the digit selected to a string (which represent the number to convert)
    {
        Serial.println("Digit selected was not added correctly");
    } 

    if (button == 'e')
    {
        inchLength = number.toFloat();
        centimeterLength = inch2cent(inchLength);
        unsigned long startTime = millis();
        presentTime = millis();
        while (presentTime < (startTime + 15000)) { //The display is activate for 15 sec
            decimalPosition = setDisplay(centimeterLength);
            presentTime = millis();
        } 
        clearDigits();
        if(decimalPosition == 0 || decimalPosition == 3) {
            while (presentTime < (startTime + 16000)) { //Flashing the DEL for 1 sec after the end of the display
                digitalWrite(delIncertitude, HIGH);
                delay(100);
                digitalWrite(delIncertitude, LOW);
                delay(100);
                presentTime = millis();
            }
        }
        else {
            while (presentTime < (startTime + 16000)) { //Flashing the DEL for 1 sec after the end of the display
                digitalWrite(delIncertitude, HIGH);
                presentTime = millis();
            }
        }
        number.remove(0); //Resetting the converter (after the 15 secondes delay needed for the display)
        numberLength = 0;
    }
}

void test() {
    float inchLength;
    unsigned long presentTime;
    int button = -1; //Reset the value to be different from the ones on the keyboard

    button = buttonSelected(); //Reading the value pressed on the keyboard 

    if (addNumber(button) == false) //Adding the digit selected to a string (which represent the number to convert)
    {
        Serial.println("Digit selected was not added correctly");
    } 

    if (button == 'e')
    {
        inchLength = number.toFloat();
        unsigned long startTime = millis();
        presentTime = millis();
        while (presentTime < (startTime + 4000)) { //The display is activate for 15 sec
            setDisplay(inchLength);
            presentTime = millis();
        } 
        clearDigits();
        number.remove(0); //Resetting the converter (after the 15 secondes delay needed for the display)
        numberLength = 0;
    }
  

    /*unsigned long present, start = millis(), millis();
    while(present < (start + 1000)){
       setDisplay(2.54);
       present = millis();
    }
    clearDigits();
    while(present < (start + 2000)){
        digitalWrite(delIncertitude, HIGH);
        delay(200);
        digitalWrite(delIncertitude, LOW);
        delay(200);
       present = millis();
    }*/
    //setDisplay(102.);
    //setDigit(0,1,0);
}
