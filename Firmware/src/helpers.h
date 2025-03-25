#ifndef src_helpers_h
#define src_helpers_h

#include <Arduino.h>

bool valueInThreshold (uint16_t val, uint16_t comp, uint16_t thr)
{
    uint16_t _absDiff = (uint16_t)abs(((int32_t)comp) - ((int32_t)val));
    return (_absDiff <= thr);
}

#endif