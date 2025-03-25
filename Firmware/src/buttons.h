#ifndef src_buttons_h
#define src_buttons_h

#include <Arduino.h>

#include "configuration.h"
#include "pins.h"
#include "helpers.h"

uint8_t btnsLast[2];
unsigned long long btnsChangeLast[2];

void btnsBegin ()
{
    for (uint8_t i = 0; i < 2; i++)
    {
        btnsLast[i] = 0;
        btnsChangeLast[i] = millis();
    }
}

uint16_t btnsReadRaw (uint8_t id)
{
    uint8_t _pin;
    switch (id)
    {
        case 1:
            _pin = PIN_BTNIN1;
            break;
        case 2:
            _pin = PIN_BTNIN2;
            break;
        default:
            return 0;
            break;
    }

    return analogRead(_pin);
}

uint8_t btnsReadBtn (uint8_t id)
{
    // get settings from config
    uint16_t _diffMax;
    uint8_t _btnsNum;
    uint16_t _btnsADC1[] = CONF_BTNS_1_CAL;
    uint16_t _btnsADC2[] = CONF_BTNS_2_CAL;
    uint16_t *_btnsADC;
    switch (id)
    {
        case 1:
            _diffMax = CONF_BTNS_1_DIFF_MAX;
            _btnsNum = CONF_BTNS_1_NUM;
            _btnsADC = _btnsADC1;
            break;
        case 2:
            _diffMax = CONF_BTNS_2_DIFF_MAX;
            _btnsNum = CONF_BTNS_2_NUM;
            _btnsADC = _btnsADC2;
            break;
        default:
            return 0;
            break;
    }

    // read raw value
    uint16_t _raw = btnsReadRaw(id);

    // check for validity
    if (!valueInThreshold(_raw, btnsReadRaw(id), _diffMax))
    {
        return 0;
    }

    // find button presses
    uint8_t _btn = 0; // 0 means no button is pressed
    for (uint8_t i = 0; i < _btnsNum; i++)
    {
        if (valueInThreshold(_raw, _btnsADC[i], _diffMax))
        {
            _btn = i + 1; // button starts at 1; 0 is no button
            break;
        }
    }

    return _btn;
}

int8_t btnsReadBtnSingleTime (uint8_t id)
{
    uint8_t _id = id - 1;

    // check if timeout has passed
    unsigned long long _now = millis();
    if (_now < (btnsChangeLast[_id] + CONF_BTNS_TIMEOUT))
    {
        return -1;
    }

    uint8_t _val = btnsReadBtn(id);
    if (_val != btnsLast[_id])
    {
        // do not allow to change from one value to another without going to 0 (NoteOff) first
        if ((btnsLast[_id] != 0) && (_val != 0))
        {
            return -1;
        }

        btnsLast[_id] = _val;
        btnsChangeLast[_id] = _now;

        return _val;
    }

    return -1;
}

#endif