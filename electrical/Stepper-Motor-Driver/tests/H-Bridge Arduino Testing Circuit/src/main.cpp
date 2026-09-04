#include <Arduino.h>

static int s_SineValues[] = {0, 6, 12, 18, 24, 30, 36, 42, 48, 54, 59, 65, 70, 75, 80, 85, 89, 94, 98, 102, 105, 108, 112, 114, 117, 119, 121, 123, 124, 125, 126, 126, 127, 126, 126, 125, 124, 123, 121, 119, 117, 114, 112, 108, 105, 102, 98, 94, 89, 85, 80, 75, 70, 65, 59, 54, 48, 42, 36, 30, 24, 18, 12, 6, 0, -7, -13, -19, -25, -31, -37, -43, -49, -55, -60, -66, -71, -76, -81, -86, -90, -95, -99, -103, -106, -109, -113, -115, -118, -120, -122, -124, -125, -126, -127, -127, -127, -127, -127, -126, -125, -124, -122, -120, -118, -115, -113, -109, -106, -103, -99, -95, -90, -86, -81, -76, -71, -66, -60, -55, -49, -43, -37, -31, -25, -19, -13, -7};
static int s_CosineValues[] = {127, 126, 126, 125, 124, 123, 121, 119, 117, 114, 112, 108, 105, 102, 98, 94, 89, 85, 80, 75, 70, 65, 59, 54, 48, 42, 36, 30, 24, 18, 12, 6, 0, -7, -13, -19, -25, -31, -37, -43, -49, -55, -60, -66, -71, -76, -81, -86, -90, -95, -99, -103, -106, -109, -113, -115, -118, -120, -122, -124, -125, -126, -127, -127, -127, -127, -127, -126, -125, -124, -122, -120, -118, -115, -113, -109, -106, -103, -99, -95, -90, -86, -81, -76, -71, -66, -60, -55, -49, -43, -37, -31, -25, -19, -13, -7, -1, 6, 12, 18, 24, 30, 36, 42, 48, 54, 59, 65, 70, 75, 80, 85, 89, 94, 98, 102, 105, 108, 112, 114, 117, 119, 121, 123, 124, 125, 126, 126};

#define ANGLE_ARRAY_SIZE 128
#define I_MAX 500

#define PIN_MOS_LEFTA_HIGH 2
#define PIN_MOS_LEFTA_LOW 22
#define PIN_MOS_RIGHTA_HIGH 3
#define PIN_MOS_RIGHTA_LOW 23

#define PIN_CURRENT_SENSORA A7
#define PIN_CURRENT_SENSORB A6

#define PIN_STEP_BUTTON 30

#define PWM_FREQ 25000

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

void setDutyCycle(int pin, float percentage);
void chargeForwardA();

static float calibrationVoltageSensorA = 2500.f;

void setup()
{
    // Timer 1 pins: OC1A (Pin 11), OC1B (Pin 12)
    // pinMode(11, OUTPUT);
    // pinMode(12, OUTPUT);

    // Timer 3 pins: OC3A (Pin 5), OC3B (Pin 2), OC3C (Pin 3)
    pinMode(PIN_MOS_RIGHTA_HIGH, OUTPUT);
    pinMode(PIN_MOS_LEFTA_HIGH, OUTPUT);
    // pinMode(5, OUTPUT);

    // Timer 4 pins: OC4A (Pin 6), OC4B (Pin 7), OC4C (Pin 8)
    // pinMode(6, OUTPUT);
    // pinMode(7, OUTPUT);
    // pinMode(8, OUTPUT);

    // Timer 5 pins: OC5A (Pin 46), OC5B (Pin 45), OC5C (Pin 44)
    // pinMode(46, OUTPUT);
    // pinMode(45, OUTPUT);
    // pinMode(44, OUTPUT);

    // Change timer 1,3,4,5 (16-bit timers) to Fast PWM and 25kHz
    // TIMER 1
    // TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
    // TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
    // ICR1   = 319;

    // TIMER 3
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3 = 0;

    TCCR3A |= (1 << COM3A1) | (1 << COM3B1) | (1 << COM3C1);
    TCCR3A |= (1 << WGM31);
    TCCR3B |= (1 << WGM33) | (1 << WGM32);

    // Set top value for 25 kHz frequency
    ICR3 = 16000000L / PWM_FREQ - 1;

    // Set prescaler to 1 (CS30 = 1)
    TCCR3B |= (1 << CS30);

    // TIMER 4
    // TCCR4A = _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1) | _BV(WGM41);
    // TCCR4B = _BV(WGM43) | _BV(WGM42) | _BV(CS40);
    // ICR4 = 319;

    // TIMER 5
    // TCCR5A = _BV(COM5A1) | _BV(COM5B1) | _BV(COM5C1) | _BV(WGM51);
    // TCCR5B = _BV(WGM53) | _BV(WGM52) | _BV(CS50);
    // ICR5   = 319;

    pinMode(PIN_MOS_LEFTA_LOW, OUTPUT);
    pinMode(PIN_MOS_RIGHTA_LOW, OUTPUT);

    Serial.begin(9600);

    delay(100);

    calibrationVoltageSensorA = analogRead(PIN_CURRENT_SENSORA) * 5000.f / 1023.f;

    digitalWrite(PIN_MOS_RIGHTA_LOW, HIGH); // Slow decay for forward direction

    setDutyCycle(PIN_MOS_RIGHTA_HIGH, 0.f);
    setDutyCycle(PIN_MOS_LEFTA_HIGH, 0.8f);
}

static int counter = 0;

void loop()
{
    float voltagemVA = analogRead(PIN_CURRENT_SENSORA) * 5000.f / 1023.f;
    float currentmAA = (voltagemVA - calibrationVoltageSensorA) / 185.f * 1000.f;

    printCurrentmA("Current current A: ", currentmAA);
    delay(300);
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

// void enableForwardB()
// {
//     digitalWrite(PIN_MOS_LEFTB_LOW, LOW);
//     digitalWrite(PIN_MOS_RIGHTB_HIGH, LOW);

//     digitalWrite(PIN_MOS_LEFTB_HIGH, HIGH);
//     digitalWrite(PIN_MOS_RIGHTB_LOW, HIGH);
// }

// void enableBackwardB()
// {
//     digitalWrite(PIN_MOS_LEFTB_HIGH, LOW);
//     digitalWrite(PIN_MOS_RIGHTB_LOW, LOW);

//     digitalWrite(PIN_MOS_LEFTB_LOW, HIGH);
//     digitalWrite(PIN_MOS_RIGHTB_HIGH, HIGH);
// }

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

// void disableForwardB()
// {
//     digitalWrite(PIN_MOS_LEFTB_HIGH, LOW);
//     digitalWrite(PIN_MOS_RIGHTB_LOW, LOW);
// }

// void disableBackwardB()
// {
//     digitalWrite(PIN_MOS_LEFTB_LOW, LOW);
//     digitalWrite(PIN_MOS_RIGHTB_HIGH, LOW);
// }

void stepButtonPressed()
{
    // TODO: Use s_StepTarget to allow for faster STEP pulses without skipping target current values

    incrementStep(&s_StepCount);
    Serial.print("New step target: ");
    Serial.println(s_StepCount);
}

void setDutyCycle(int pin, float percentage)
{
    if (percentage == 0.f)
    {
        pinMode(pin, INPUT);
        return;
    }

    pinMode(pin, OUTPUT);

    int dutyVal = (int)(percentage * (16000000L / PWM_FREQ - 1));

    if (pin == PIN_MOS_LEFTA_HIGH)
        OCR3B = dutyVal;
    else if (pin == PIN_MOS_RIGHTA_HIGH)
        OCR3C = dutyVal;
    else
    {
        Serial.print("Wrong pin in setDutyCycle: ");
        Serial.println(pin);
    }
}

void chargeForwardA()
{
    digitalWrite(PIN_MOS_LEFTA_HIGH, LOW);
    digitalWrite(PIN_MOS_LEFTA_LOW, LOW);
    digitalWrite(PIN_MOS_RIGHTA_HIGH, LOW);
    digitalWrite(PIN_MOS_RIGHTA_LOW, LOW);

    delayMicroseconds(10);

    digitalWrite(PIN_MOS_RIGHTA_LOW, HIGH);
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