/*
 * crc_cal.h
 *
 *  Created on: Oct 7, 2026
 *      Author: minh-bui
 */

#ifndef INC_CRC_CAL_H_
#define INC_CRC_CAL_H_

#include "stm32f1xx.h"

uint32_t cal_hw_crc32 (uint32_t *data, uint32_t length_in_words);
#endif /* INC_CRC_CAL_H_ */
