#ifndef inc_configuration_h
#define inc_configuration_h

// ** SECTION: MIDI **

// CC control addresses to send to from AIN1 to AIN5
/** Default Mapping:
 *      AIN1: Modulation (01)
 *      AIN2: Breath / (Aftertouch) (02)
 *      AIN3: Foot Controller (04)
 *      AIN4: General Purpose 1 (16)
 *      AIN5: Sound Controller 2 / (VCF / Filter) (71)
*/
#define CONF_MIDI_AIN_MAP {1, 2, 4, 16, 71}

// MIDI note values (1-127) for button controller 1
// default: {60, 61, 62, 63, 64, 65} (around middle C)
#define CONF_MIDI_BTNS_1_MAP {60, 61, 62, 63, 64, 65}

// MIDI note values (1-127) for button controller 2
// default: {}
#define CONF_MIDI_BTNS_2_MAP {}

// MIDI channel to use for AIN: usually the same as the keyboard to be controlled
// default: 1
#define CONF_MIDI_CH_AIN 1

// MIDI channel to use for BTNS: usually NOT the one that the keyboard(s) use
// default: 3 (to allow for two keyboards, as many people have it)
#define CONF_MIDI_CH_BTNS 3

// threshold of Input movement to open MIDI send-gate (all AIN range 0-1023 when calibrated correctly)
// default: 5
#define CONF_MIDI_SEND_THRESHOLD 5

// timeout to close gate after last movement (hold-time) [ms]
// default: 300
#define CONF_MIDI_SEND_TIMEOUT 300


// ** SECTION: Analog Inputs **

// minimal difference between CAL max and min value for a AIN channel to be considered active (input values can range 0-1023)
// default: 50
#define CONF_AIN_CAL_DIFF_MIN 50

// maximal ADC input value for a AIN channel to be considered connected (without connection an input gets pulled to AREF -> 1023 in ideal scenario)
// default: 1020
#define CONF_AIN_CONNECTED_VAL_MAX 1020


// ** SECTION: Buttonboards **

// maximum number of buttons used by any controller
// default: 6 (Hand-Controller)
#define CONF_BTNS_NUM_MAX 6

// timeout to wait for after a button was pressed [ms]
// default: 300
#define CONF_BTNS_TIMEOUT 300

// number of buttons on controller 1
// default: 6 (Hand-Controller)
#define CONF_BTNS_1_NUM 6

// ADC values of buttons on controller 1
// default: {92, 177, 324, 510, 695, 931}
#define CONF_BTNS_1_CAL {92, 177, 324, 510, 695, 931}

// ADC value difference for a button to be recognized on controller 1
// default: 40
#define CONF_BTNS_1_DIFF_MAX 40

// number of buttons on controller 2
// default: 0 (OFF)
#define CONF_BTNS_2_NUM 0

// ADC values of buttons on controller 2
// default: {}
#define CONF_BTNS_2_CAL {}

// ADC value difference for a button to be recognized on controller 2
// default: 40
#define CONF_BTNS_2_DIFF_MAX 40



// ** SECTION: Debugging **

// uncomment to enable debugging via USB-Serial
//#define CONF_DEB_EN

// uncomment to disable MIDI output (-> speeds up Serial debugging when no MIDI software connected)
//#define CONF_DEB_DISABLE_MIDI

// baud rate for USB-Serial
#define CONF_DEB_BAUD 115200

#endif