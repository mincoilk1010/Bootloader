/*
 * flash_prog.h
 *
 *  Created on: Oct 5, 2026
 *      Author: Admin
 */

#ifndef INC_FLASH_PROG_H_
#define INC_FLASH_PROG_H_

#include "stm32f1xx_hal.h"
#include "type_config.h"

int flash_erase(u32 addr, u32 pages);
int flash_write(uint32_t addr, const uint8_t *d, uint32_t len);

#endif /* INC_FLASH_PROG_H_ */
