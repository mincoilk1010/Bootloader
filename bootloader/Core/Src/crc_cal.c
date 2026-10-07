/*
 * crc_cal.c
 *
 *  Created on: Oct 7, 2026
 *      Author: minh-bui
 */

#include "crc_cal.h"

uint32_t cal_hw_crc32 (uint32_t *data, uint32_t length_in_words) {
    CRC->CR = CRC_CR_RESET;

    for(uint32_t i = 0; i < length_in_words; i++) {
        CRC->DR = data[i];
    }
    return CRC->DR;
}