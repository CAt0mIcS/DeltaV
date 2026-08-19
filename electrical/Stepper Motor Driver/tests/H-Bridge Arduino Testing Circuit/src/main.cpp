#include <Arduino.h>

static int s_SineValues[] = {0, 6, 12, 18, 24, 30, 36, 42, 48, 54, 59, 65, 70, 75, 80, 85, 89, 94, 98, 102, 105, 108, 112, 114, 117, 119, 121, 123, 124, 125, 126, 126, 127, 126, 126, 125, 124, 123, 121, 119, 117, 114, 112, 108, 105, 102, 98, 94, 89, 85, 80, 75, 70, 65, 59, 54, 48, 42, 36, 30, 24, 18, 12, 6, 0, -7, -13, -19, -25, -31, -37, -43, -49, -55, -60, -66, -71, -76, -81, -86, -90, -95, -99, -103, -106, -109, -113, -115, -118, -120, -122, -124, -125, -126, -127, -127, -127, -127, -127, -126, -125, -124, -122, -120, -118, -115, -113, -109, -106, -103, -99, -95, -90, -86, -81, -76, -71, -66, -60, -55, -49, -43, -37, -31, -25, -19, -13, -7};
static int s_CosineValues[] = {127, 126, 126, 125, 124, 123, 121, 119, 117, 114, 112, 108, 105, 102, 98, 94, 89, 85, 80, 75, 70, 65, 59, 54, 48, 42, 36, 30, 24, 18, 12, 6, 0, -7, -13, -19, -25, -31, -37, -43, -49, -55, -60, -66, -71, -76, -81, -86, -90, -95, -99, -103, -106, -109, -113, -115, -118, -120, -122, -124, -125, -126, -127, -127, -127, -127, -127, -126, -125, -124, -122, -120, -118, -115, -113, -109, -106, -103, -99, -95, -90, -86, -81, -76, -71, -66, -60, -55, -49, -43, -37, -31, -25, -19, -13, -7, -1, 6, 12, 18, 24, 30, 36, 42, 48, 54, 59, 65, 70, 75, 80, 85, 89, 94, 98, 102, 105, 108, 112, 114, 117, 119, 121, 123, 124, 125, 126, 126};

#define ANGLE_ARRAY_SIZE 128
#define I_MAX 500

#define PIN_MOS_LEFTA_HIGH 25
#define PIN_MOS_LEFTA_LOW 24
#define PIN_MOS_RIGHTA_HIGH 22
#define PIN_MOS_RIGHTA_LOW 23

#define PIN_MOS_LEFTB_HIGH 29
#define PIN_MOS_LEFTB_LOW 28
#define PIN_MOS_RIGHTB_HIGH 26
#define PIN_MOS_RIGHTB_LOW 27

#define PIN_CURRENT_SENSORA A3
#define PIN_CURRENT_SENSORB A4

#define PIN_STEP_BUTTON 30

static int s_StepCount = 0;
static int s_StepTarget = 0;

void incrementStep(int *count);
void printCurrentmA(const char *text, float current);
void printVoltagemV(const char *text, float voltage);

void enableForwardA();
void enableBackwardA();
void enableForwardB();
void enableBackwardB();

void disableForwardA();
void disableBackwardA();
void disableForwardB();
void disableBackwardB();

void stepButtonPressed();

void setup()
{
    pinMode(PIN_MOS_LEFTA_HIGH, OUTPUT);
    pinMode(PIN_MOS_LEFTA_LOW, OUTPUT);
    pinMode(PIN_MOS_RIGHTA_HIGH, OUTPUT);
    pinMode(PIN_MOS_RIGHTA_LOW, OUTPUT);

    pinMode(PIN_MOS_LEFTB_HIGH, OUTPUT);
    pinMode(PIN_MOS_LEFTB_LOW, OUTPUT);
    pinMode(PIN_MOS_RIGHTB_HIGH, OUTPUT);
    pinMode(PIN_MOS_RIGHTB_LOW, OUTPUT);

    pinMode(PIN_STEP_BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_STEP_BUTTON), stepButtonPressed, FALLING);

    Serial.begin(9600);
}

void loop()
{
    float voltagemVA = analogRead(PIN_CURRENT_SENSORA) * 5000.f / 1023.f;
    float currentmAA = (voltagemVA - 2500.f) / 185.f;

    printCurrentmA("Current current A: ", currentmAA);

    float voltagemVB = analogRead(PIN_CURRENT_SENSORB) * 5000.f / 1023.f;
    float currentmAB = (voltagemVB - 2500.f) / 185.f;

    printCurrentmA("Current current B: ", currentmAB);

    float coilATarget = s_SineValues[s_StepCount] / 127.f * I_MAX;
    float coilBTarget = s_CosineValues[s_StepCount] / 127.f * I_MAX;
    printCurrentmA("Target current A: ", coilATarget);
    printCurrentmA("Target current B: ", coilBTarget);

    if (coilATarget >= 0.f)
    {
        if (currentmAA < coilATarget - 50)
            enableForwardA();
        else if (currentmAA > coilATarget + 50)
            disableForwardA();
    }
    else
    {
        if (currentmAA > coilATarget + 50)
            enableBackwardA();
        else if (currentmAA < coilATarget - 50)
            disableBackwardA();
    }

    if (coilBTarget >= 0.f)
    {
        if (currentmAB < coilBTarget - 50)
            enableForwardB();
        if (currentmAB > coilBTarget + 50)
            disableForwardB();
    }
    else
    {
        if (currentmAB > coilBTarget + 50)
            enableBackwardB();
        else if (currentmAB < coilBTarget - 50)
            disableBackwardB();
    }
}

void incrementStep(int *count)
{
    (*count)++;
    if ((*count) >= ANGLE_ARRAY_SIZE)
        (*count) = 0;
}

void enableForwardA()
{
    digitalWrite(PIN_MOS_LEFTA_LOW, LOW);
    digitalWrite(PIN_MOS_RIGHTA_HIGH, LOW);

    // Deadtime
    delayMicroseconds(1);

    digitalWrite(PIN_MOS_LEFTA_HIGH, HIGH);
    digitalWrite(PIN_MOS_RIGHTA_LOW, HIGH);
}

void enableBackwardA()
{
    digitalWrite(PIN_MOS_LEFTA_HIGH, LOW);
    digitalWrite(PIN_MOS_RIGHTA_LOW, LOW);

    // Deadtime
    delayMicroseconds(1);

    digitalWrite(PIN_MOS_LEFTA_LOW, HIGH);
    digitalWrite(PIN_MOS_RIGHTA_HIGH, HIGH);
}

void enableForwardB()
{
    digitalWrite(PIN_MOS_LEFTB_LOW, LOW);
    digitalWrite(PIN_MOS_RIGHTB_HIGH, LOW);

    // Deadtime
    delayMicroseconds(1);

    digitalWrite(PIN_MOS_LEFTB_HIGH, HIGH);
    digitalWrite(PIN_MOS_RIGHTB_LOW, HIGH);
}

void enableBackwardB()
{
    digitalWrite(PIN_MOS_LEFTB_HIGH, LOW);
    digitalWrite(PIN_MOS_RIGHTB_LOW, LOW);

    // Deadtime
    delayMicroseconds(1);

    digitalWrite(PIN_MOS_LEFTB_LOW, HIGH);
    digitalWrite(PIN_MOS_RIGHTB_HIGH, HIGH);
}

void disableForwardA()
{
    digitalWrite(PIN_MOS_LEFTA_HIGH, LOW);
    digitalWrite(PIN_MOS_RIGHTA_LOW, LOW);
}

void disableBackwardA()
{
    digitalWrite(PIN_MOS_LEFTA_LOW, LOW);
    digitalWrite(PIN_MOS_RIGHTA_HIGH, LOW);
}

void disableForwardB()
{
    digitalWrite(PIN_MOS_LEFTB_HIGH, LOW);
    digitalWrite(PIN_MOS_RIGHTB_LOW, LOW);
}

void disableBackwardB()
{
    digitalWrite(PIN_MOS_LEFTB_LOW, LOW);
    digitalWrite(PIN_MOS_RIGHTB_HIGH, LOW);
}

void stepButtonPressed()
{
    // TODO: Use s_StepTarget to allow for faster STEP pulses without skipping target current values

    incrementStep(&s_StepCount);
    Serial.print("New step target: ");
    Serial.println(s_StepCount);
}

void printCurrentmA(const char *text, float current)
{
    Serial.print(text);
    Serial.print(current);
    Serial.println(" mA");
}

void printVoltagemV(const char *text, float voltage)
{
    Serial.print(text);
    Serial.print(voltage);
    Serial.println(" mV");
}