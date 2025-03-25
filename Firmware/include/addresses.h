#ifndef inc_addresses_h
#define inc_addresses_h

/// @brief AIN calibration values
/// @note All values have a 16-bit width and therefore need two cells of EEPROM per value stored
#define ADR_AIN1_MIN 10
#define ADR_AIN1_MAX 12
#define ADR_AIN2_MIN 20
#define ADR_AIN2_MAX 22
#define ADR_AIN3_MIN 30
#define ADR_AIN3_MAX 32
#define ADR_AIN4_MIN 40
#define ADR_AIN4_MAX 42
#define ADR_AIN5_MIN 50
#define ADR_AIN5_MAX 52
#define ADR_AIN_ALL {ADR_AIN1_MIN, ADR_AIN1_MAX, ADR_AIN2_MIN, ADR_AIN2_MAX, ADR_AIN3_MIN, ADR_AIN3_MAX, ADR_AIN4_MIN, ADR_AIN4_MAX, ADR_AIN5_MIN, ADR_AIN5_MAX}

#endif