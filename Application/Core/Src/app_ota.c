/*
 * app_ota.c
 *
 *  Created on: Oct 9, 2026
 *      Author: minh-bui
 */

#include "app_ota.h"
#include "flash_operations.h"
#include "flash_layout.h"
#include "main.h"

#define OTA_FLAG_START 1

uint32_t flash_buffer[5];

void enable_ota_request (void) {
	HAL_FLASH_Unlock();
	flash_read_page(APP_HEADER_ADDR, flash_buffer);
	flash_buffer[0] = OTA_FLAG_START;
	flash_erase_page(APP_HEADER_ADDR);
	flash_write_page(APP_HEADER_ADDR, flash_buffer);
	HAL_FLASH_Lock();
	NVIC_SystemReset();
}

