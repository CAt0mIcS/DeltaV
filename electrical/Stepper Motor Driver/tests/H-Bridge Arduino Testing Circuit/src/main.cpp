#include <Arduino.h>

static int s_SineValues[] = {0, 6, 12, 19, 25, 31, 38, 44, 50, 55, 61, 67, 72, 77, 82, 87, 92, 96, 100, 104, 107, 111, 114, 116, 119, 121, 122, 124, 125, 126, 126, 127, 126, 126, 125, 124, 122, 121, 119, 116, 114, 111, 107, 104, 100, 96, 92, 87, 82, 77, 72, 67, 61, 55, 50, 44, 38, 31, 25, 19, 12, 6, 0, -6, -12, -19, -25, -31, -38, -44, -50, -55, -61, -67, -72, -77, -82, -87, -92, -96, -100, -104, -107, -111, -114, -116, -119, -121, -122, -124, -125, -126, -126, -127, -126, -126, -125, -124, -122, -121, -119, -116, -114, -111, -107, -104, -100, -96, -92, -87, -82, -77, -72, -67, -61, -55, -50, -44, -38, -31, -25, -19, -12, -6, 0};
static int s_CosineValues[] = {127, 126, 126, 125, 124, 122, 121, 119, 116, 114, 111, 107, 104, 100, 96, 92, 87, 82, 77, 72, 67, 61, 55, 50, 44, 38, 31, 25, 19, 12, 6, 0, -6, -12, -19, -25, -31, -38, -44, -50, -55, -61, -67, -72, -77, -82, -87, -92, -96, -100, -104, -107, -111, -114, -116, -119, -121, -122, -124, -125, -126, -126, -127, -126, -126, -125, -124, -122, -121, -119, -116, -114, -111, -107, -104, -100, -96, -92, -87, -82, -77, -72, -67, -61, -55, -50, -44, -38, -31, -25, -19, -12, 0, 6, 12, 19, 25, 31, 38, 44, 50, 55, 61, 67, 72, 77, 82, 87, 92, 96, 100, 104, 107, 111, 114, 116, 119, 121, 122, 124, 125, 126, 126, 127};

#define ANGLE_ARRAY_SIZE 125
#define I_MAX 1800

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

static int s_StepCount = 0;

void incrementStepCount();
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

    Serial.begin(9600);
}

void loop()
{
    float voltagemVA = analogRead(PIN_CURRENT_SENSORA) * 5000.f / 1023.f;
    float currentmAA = (voltagemVA - 2500.f) / 185.f;

    printCurrentmA("Current current A: ", currentmAA);

    float voltagemVB = analogRead(PIN_CURRENT_SENSORB) * 5000.f / 1023.f;
    float currentmAB = (voltagemVB - 2500.f) / 185.f;

    printCurrentmA("Current current B: ", currentmAA);

    float coilATarget = s_SineValues[s_StepCount] / 127.f * I_MAX;
    float coilBTarget = s_CosineValues[s_StepCount] / 127.f * I_MAX;
    printCurrentmA("Target current A: ", coilATarget);
    printCurrentmA("Target current B: ", coilBTarget);

    if (coilATarget >= 0.f)
    {
        if (currentmAA < coilATarget)
            enableForwardA();
        else if (currentmAA > coilATarget)
            disableForwardA();
    }
    else
    {
        if (currentmAA > coilATarget)
            enableBackwardA();
        else if (currentmAA < coilATarget)
            disableBackwardA();
    }

    if (coilBTarget >= 0.f)
    {
        if (currentmAB < coilBTarget)
            enableForwardB();
        if (currentmAB > coilBTarget)
            disableForwardB();
    }
    else
    {
        if (currentmAB > coilBTarget)
            enableBackwardB();
        else if (currentmAB < coilBTarget)
            disableBackwardB();
    }

    delay(1);
}

void incrementStepCount()
{
    s_StepCount++;
    if (s_StepCount >= ANGLE_ARRAY_SIZE)
        s_StepCount = 0;
}

void enableForwardA()
{
    digitalWrite(PIN_MOS_LEFTA_LOW, LOW);
    digitalWrite(PIN_MOS_RIGHTA_HIGH, LOW);
    digitalWrite(PIN_MOS_LEFTA_HIGH, HIGH);
    digitalWrite(PIN_MOS_RIGHTA_LOW, HIGH);
}

void enableBackwardA()
{
    digitalWrite(PIN_MOS_LEFTA_HIGH, LOW);
    digitalWrite(PIN_MOS_RIGHTA_LOW, LOW);
    digitalWrite(PIN_MOS_LEFTA_LOW, HIGH);
    digitalWrite(PIN_MOS_RIGHTA_HIGH, HIGH);
}

void enableForwardB()
{
    digitalWrite(PIN_MOS_LEFTB_LOW, LOW);
    digitalWrite(PIN_MOS_RIGHTB_HIGH, LOW);
    digitalWrite(PIN_MOS_LEFTB_HIGH, HIGH);
    digitalWrite(PIN_MOS_RIGHTB_LOW, HIGH);
}

void enableBackwardB()
{
    digitalWrite(PIN_MOS_LEFTB_HIGH, LOW);
    digitalWrite(PIN_MOS_RIGHTB_LOW, LOW);
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