#include "TM1638.h"

#define trigPin    12
#define echoPin    11
#define strobePin  10
#define clockPin    9
#define dataPin     8
#define numberOfCalculations   50

// Global variables
TM1638 module(dataPin, clockPin, strobePin);
boolean showLines = true;
float samples[numberOfCalculations];

void setup() {
    pinMode(trigPin, OUTPUT); //Pin, do którego podłączymy trig jako wyjście
    pinMode(echoPin, INPUT);  //a echo, jako wejście
    digitalWrite(trigPin, LOW);
    delayMicroseconds(500);
    module.setDisplayToString("--------");
}

void loop() {
    byte buttons = module.getButtons();
    if(showLines){
        module.setDisplayToString("--------");
        delay(100);
        module.clearDisplay();
        delay(100);
    }

    if(buttons==1){
        float tmp = calculateDistance();
        module.setDisplayToDecNumber(tmp * 10,2,false);
        showLines=false;
        delay(250);
    }
    if(buttons==2)
        showLines = true;
}


// Set values of samples from Ultrasonic distance sensor HC-SR04
void setSamples(){
    for(int i = 0; i< numberOfCalculations; ++i){
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        samples[i]= pulseIn(echoPin, HIGH) / 57.7;
        delay(5);
    }
}

// Calculate mean of samples
float mean(){
    float mean = 0;
    for(byte i = 0; i < numberOfCalculations; ++i)
        mean += samples[i];
    return mean/numberOfCalculations;
}

// Calculate standard deviation od samples
float standardDeviation(float _mean){
    float variance = 0;
    for(byte i = 0; i < numberOfCalculations; ++i)
        variance += sq(samples[i] - _mean);
    return sqrt(variance/numberOfCalculations);
}


// Calculate distance as an average of samles,
// that are no further than 3 standard deviations 
// from mean value. (Three sigmas rule)
float calculateDistance(){
    setSamples();
    float _mean = mean();
    float _std = standardDeviation(_mean);
    float avg = 0;
    int num = 0;

    for(byte i = 0; i< numberOfCalculations; ++i){
        if(!(samples[i] > _mean + 3*_std || samples[i] < _mean  - 3*_std)){
            avg += samples[i];
            num++;
        }
    }
    return avg/num;
}
