#include <Arduino.h>

#include "dio.h"
#include "analog.h"
#include "buttons.h"
#include "midi.h"

#include "configuration.h"

void setup() {
    // debugging interface
    #ifdef CONF_DEB_EN
    Serial.begin(CONF_DEB_BAUD);
    #endif

    // setup pins
    pinsSetupAll();

    // setup MIDI
    midiBegin(CONF_MIDI_CH_AIN, CONF_MIDI_CH_BTNS);

    // load CAL data
    loadEEPROMCalAll();

    // initialize buttons handler
    btnsBegin();

    // blink for startup finish confirmation
    ledBlinkFast();
}

void loop() {
    // check for CAL button press
    if (btnCalRead())
    {
        ledOn();
        delay(200);
        ledOff();
        if (btnCalRead())
        {
            calRun();
        }
    }

    // send analog MIDI data
    for (uint8_t i = 1; i < 6; i++)
    {
        if (ainActive(i) && ainConnected(i))
        {
            midiSendAin(i, ainReadNormval(i));
        }
    }

    // send button MIDI data
    for (uint8_t i = 1; i < 3; i++)
    {
        midiSendBtns(i, btnsReadBtnSingleTime(i));
    }

    // wait for MIDI buffer to be emptied
    midiFlush();
}
