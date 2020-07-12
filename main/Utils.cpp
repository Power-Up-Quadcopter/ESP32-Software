//
// Created by Allen on 7/11/2020.
//

#include <cstdint>
#include <math.h>
#include "Utils.h"

int32_t unsignedToSigned(uint32_t num, int bits) {
    if(num >= pow(2,bits-1))
        return num-pow(2,bits);
    return num;
}
