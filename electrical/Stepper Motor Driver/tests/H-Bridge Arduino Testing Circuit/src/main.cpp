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

#define PIN_CURRENT_SENSOR A3

static int s_StepCount = 0;

void incrementStepCount();

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
}

void loop()
{
    int currentCurrent = analogRead(PIN_CURRENT_SENSOR);

    int coilATarget = s_SineValues[s_StepCount] / 127.f * I_MAX;
    int coilBTarget = s_CosineValues[s_StepCount] / 127.f * I_MAX;
}

void incrementStepCount()
{
    s_StepCount++;
    if (s_StepCount >= ANGLE_ARRAY_SIZE)
        s_StepCount = 0;
}