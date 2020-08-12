//
// Created by Allen on 7/11/2020.
//

#ifndef POWERUP_UTILS_H
#define POWERUP_UTILS_H

/** Properly converts a unsigned integer to a signed integer, provided the value isn't to large to fit.
 *
 * @param num The number to convert.
 * @param bits The size of the number in bits.
 * @return A signed integer with the same value as the input.
 */
extern int32_t unsignedToSigned(uint32_t num, int bits);

#endif //POWERUP_UTILS_H
