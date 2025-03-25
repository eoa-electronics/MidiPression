#ifndef src_io_h
#define src_io_h

#include <Arduino.h>

#include "pins.h"

void pinModeOut (uint8_t pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void pinsSetupAll ()
{
    pinModeOut(PIN_LED);
    pinMode(PIN_BTN_CAL, INPUT_PULLUP);
}

void ledOn ()
{
    digitalWrite(PIN_LED, HIGH);
}

void ledOff ()
{
    digitalWrite(PIN_LED, LOW);
}

void ledBlink ()
{
    for(uint8_t i = 0; i < 5; i++)
    {
        ledOn();
        delay(100);
        ledOff();
        delay(100);
    }
}

void ledBlinkFast ()
{
    for(uint8_t i = 0; i < 10; i++)
    {
        ledOn();
        delay(50);
        ledOff();
        delay(50);
    }
}

bool btnCalRead ()
{
    return !digitalRead(PIN_BTN_CAL);
}

#endif