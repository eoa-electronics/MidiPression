#ifndef src_midi_h
#define src_midi_h

#include <Arduino.h>
#include <MIDIUSB.h>

#include "configuration.h"
#include "helpers.h"
#include "dio.h"

uint8_t midiChAin;
uint8_t midiChBtns;
uint16_t ctrlNormValsLast[5];
uint8_t ctrlMidiValsLast[5];
unsigned long ctrlTimeLast[5];
int8_t btnsLastPressed[2];

void midiGateClear ()
{
    for (uint8_t i = 0; i < 5; i++)
    {
        ctrlNormValsLast[i] = 0;
        ctrlMidiValsLast[i] = 0;
    }
}

void midiTimeoutClear ()
{
    for (uint8_t i = 0; i < 5; i++)
    {
        ctrlTimeLast[i] = millis();
    }
}

void midiBtnsClear ()
{
    for (uint8_t i = 0; i < 2; i++)
    {
        btnsLastPressed[i] = 0;
    }
}

void midiSetCh(uint8_t chAin, uint8_t chBtns)
{
    if ((chAin < 1) || (chAin > 16))
    {
        return;
    }
    midiChAin = chAin - 1; // MidiUSB needs CH from 0 to 15; real MIDI is 1-16

    if ((chBtns < 1) || (chBtns > 16))
    {
        return;
    }
    midiChBtns = chBtns - 1; // MidiUSB needs CH from 0 to 15; real MIDI is 1-16
}

void midiBegin (uint8_t chAin, uint8_t chBtns)
{
    midiSetCh(chAin, chBtns);
    midiGateClear();
    midiTimeoutClear();
}

void midiWriteCC (uint8_t ch, uint8_t ctrl, uint8_t val)
{
    ledOn(); // turn on LED to signal an input has happened

    // generate first MIDI byte
    uint8_t _b1 = 0xb0;
    _b1 = _b1 | (ch & 0x0f);

    // send MIDI over USB
    #ifndef CONF_DEB_DISABLE_MIDI
    midiEventPacket_t midiCC = {0x0b, _b1, ctrl, val};
    MidiUSB.sendMIDI(midiCC);
    #endif

    // send debugging output
    #ifdef CONF_DEB_EN
    Serial.print("CC Out: Ch: ");
    Serial.print(ch);
    Serial.print("; Ctrl: ");
    Serial.print(ctrl);
    Serial.print("; Val: ");
    Serial.println(val);
    #endif

    ledOff(); // turn LED off again
}

void midiSendCC (uint8_t ch, uint8_t id, uint16_t normVal)
{   
    // id to address
    uint8_t _id = id - 1;

    // check gate threshold
    if (!valueInThreshold(normVal, ctrlNormValsLast[_id], CONF_MIDI_SEND_THRESHOLD))
    {
        ctrlTimeLast[_id] = millis();
    }

    // reset timer
    unsigned long _timer = millis() - ctrlTimeLast[_id];

    // check for input still moving
    bool _moving = _timer < CONF_MIDI_SEND_TIMEOUT;
    if (!_moving)
    {
        return;
    }
    // check if value really changed (may not have due to timeout or resolution reduction)
    if (ctrlNormValsLast[_id] == normVal)
    {
        return;
    }

    // send to ctrl according to mapping
    uint8_t _midiVal = map(normVal, 0, 1023, 0, 127);
    if (ctrlMidiValsLast[_id] == _midiVal)
    {
        return;
    }
    uint8_t _ccMap[] = CONF_MIDI_AIN_MAP;
    midiWriteCC(midiChAin, _ccMap[_id], _midiVal);

    // store new value for gate and MIDI-filter
    ctrlNormValsLast[_id] = normVal;
    ctrlMidiValsLast[_id] = _midiVal;
}

void midiWriteNoteOn (uint8_t ch, uint8_t noteVal, uint8_t velocity)
{
    ledOn(); // turn on LED to signal an input has happened

    // generate first MIDI byte
    uint8_t _b1 = 0x90;
    _b1 = _b1 | (ch & 0x0f);

    // send MIDI over USB
    #ifndef CONF_DEB_DISABLE_MIDI
    midiEventPacket_t midiCC = {0x09, _b1, noteVal, velocity};
    MidiUSB.sendMIDI(midiCC);
    #endif

    // send debugging output
    #ifdef CONF_DEB_EN
    Serial.print("NoteOn Out: Ch: ");
    Serial.print(ch);
    Serial.print("; NoteVal: ");
    Serial.print(noteVal);
    Serial.print("; Vel: ");
    Serial.println(velocity);
    #endif

    ledOff(); // turn LED off again
}

void midiWriteNoteOff (uint8_t ch, uint8_t noteVal)
{
    ledOn(); // turn on LED to signal an input has happened

    // generate first MIDI byte
    uint8_t _b1 = 0x80;
    _b1 = _b1 | (ch & 0x0f);

    // send MIDI over USB
    #ifndef CONF_DEB_DISABLE_MIDI
    midiEventPacket_t midiCC = {0x08, _b1, noteVal, 0x00};
    MidiUSB.sendMIDI(midiCC);
    #endif

    // send debugging output
    #ifdef CONF_DEB_EN
    Serial.print("NoteOff Out: Ch: ");
    Serial.print(ch);
    Serial.print("; NoteVal: ");
    Serial.println(noteVal);
    #endif

    ledOff(); // turn LED off again
}

void midiSendAin (uint8_t id, uint16_t normVal)
{
    midiSendCC(midiChAin, id, normVal);
}

void midiSendBtns (uint8_t id, int8_t btn)
{
    // check if button has changed
    if (btn < 0)
    {
        return;
    }

    // get mapping
    uint8_t _map1[] = CONF_MIDI_BTNS_1_MAP;
    uint8_t _map2[] = CONF_MIDI_BTNS_2_MAP;
    uint8_t *_map;
    switch (id)
    {
        case 1:
            _map = _map1;
            break;
        case 2:
            _map = _map2;
            break;
        default:
            return;
            break;
    }

    uint8_t _id = id - 1;

    if (btn == 0) // note off
    {
        uint8_t _note = _map[btnsLastPressed[_id]-1];
        midiWriteNoteOff(midiChBtns, _note);
    }
    else // note on
    {
        btnsLastPressed[_id] = btn;
        uint8_t _note = _map[btn-1];
        midiWriteNoteOn(midiChBtns, _note, 127);
    }
}

void midiFlush ()
{
    MidiUSB.flush();
}

#endif