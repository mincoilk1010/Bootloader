/*
 * crc32.h
 *
 *  Created on: Sep 30, 2026
 *      Author: Admin
 */

#ifndef INC_CRC32_H_
#define INC_CRC32_H_
#include "bl_jump.h"
uint32_t crc32(const uint8_t *data, uint32_t length);
uint16_t crc16(uint16_t c, const uint8_t *p, uint32_t n);
#endif /* INC_CRC32_H_ */
