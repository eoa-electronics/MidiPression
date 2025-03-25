#ifndef src_analog_h
#define src_analog_h

#include <Arduino.h>
#include <EEPROM.h>

#include "configuration.h"
#include "addresses.h"
#include "dio.h"

uint16_t calAin[10];
uint16_t calAinAdrs[] = ADR_AIN_ALL;

uint16_t ainReadRaw (uint8_t id)
{
    uint8_t _pin;
    switch (id)
    {
        case 1:
            _pin = PIN_AIN1;
            break;
        case 2:
            _pin = PIN_AIN2;
            break;
        case 3:
            _pin = PIN_AIN3;
            break;
        case 4:
            _pin = PIN_AIN4;
            break;
        case 5:
            _pin = PIN_AIN5;
            break;
        default:
            return 0;
            break;
    }

    return analogRead(_pin);
}

void loadEEPROMCalAll ()
{
    for (uint8_t i = 0; i < 10; i++)
    {
        uint16_t _buf;
        EEPROM.get(calAinAdrs[i], _buf);
        calAin[i] = _buf;
    }
}

void saveEEPROMCalAll ()
{
    for (uint8_t i = 0; i < 10; i++)
    {
        uint16_t _buf = calAin[i];
        EEPROM.put(calAinAdrs[i], _buf);
    }
}

void calRun ()
{
    ledBlink(); // blink CAL LED
    ledOn(); // turn LED on to show CAL active

    // reset CAL data
    for (uint8_t i = 1; i < 6; i++)
    {
        calAin[((i-1)*2)] = ainReadRaw(i);
        calAin[((i-1)*2)+1] = ainReadRaw(i);
    }

    bool _run = true;
    while (_run)
    {
        // exit on CAL button press
        if (btnCalRead())
        {
            ledOff();
            delay(50);
            ledOn();
            if (btnCalRead())
            {
                _run = false;
                continue;
            }
        }

        // calibrate all 5 channels at once
        for (uint8_t i = 1; i < 6; i++)
        {
            calAin[((i-1)*2)] = min(calAin[((i-1)*2)], ainReadRaw(i));
            calAin[((i-1)*2)+1] = max(calAin[((i-1)*2)+1], ainReadRaw(i));
        }
    }

    ledOff(); // turn CAL LED off again

    saveEEPROMCalAll(); // save to EEPROM

    ledBlink(); // blink for confirmation
}

uint16_t ainReadNormval (uint8_t id)
{
    return max(min(map(ainReadRaw(id), calAin[((id-1)*2)], calAin[((id-1)*2)+1], 0, 1024), 1023), 0);
}

bool ainActive (uint8_t id)
{
    uint16_t _calDiff = calAin[((id-1)*2)+1] - calAin[((id-1)*2)];
    return _calDiff >= CONF_AIN_CAL_DIFF_MIN;
}

bool ainConnected (uint8_t id)
{
    return ainReadRaw(id) <= CONF_AIN_CONNECTED_VAL_MAX;
}

#endif