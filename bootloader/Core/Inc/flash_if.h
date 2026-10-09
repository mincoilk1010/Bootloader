/*
 * flash_if.h
 *
 *  Created on: Oct 9, 2026
 *      Author: DELL
 */

#ifndef INC_FLASH_IF_H_
#define INC_FLASH_IF_H_

#include "stm32f1xx_hal.h"

// Địa chỉ bắt đầu của App (Page 16 - 0x08004000)
#define APP_START_ADDRESS    ((uint32_t)0x08004000)
#define FLASH_PAGE_SIZE_F1   1024 // 1 KB mỗi page cho STM32F103C8

uint8_t Flash_EraseAppSpace(uint32_t app_size);
uint8_t Flash_WriteData(uint32_t address, uint8_t *pData, uint32_t length);

#endif /* INC_FLASH_IF_H_ */
