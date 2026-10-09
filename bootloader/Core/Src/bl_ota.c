/*
 * bl_ota.c
 *
 *  Created on: Oct 9, 2026
 *      Author: minh-bui
 */

#include "bl_ota.h"
#include "flash_operations.h"
#include "flash_layout.h"
#include "main.h"

#define OTA_FLAG_START 1
#define OTA_FLAG_CLEAR 0

uint32_t flash_buffer[5];

int check_ota_request (void)
{
    flash_read_page(APP_HEADER_ADDR, flash_buffer);

    if (flash_buffer[0] == OTA_FLAG_START)
        return 0;

    return 1;
}

void clear_ota_flag (void) {
	HAL_FLASH_Unlock();
	flash_read_page(APP_HEADER_ADDR, flash_buffer);
	flash_buffer[0] = OTA_FLAG_CLEAR;
	flash_erase_page(APP_HEADER_ADDR);
	flash_write_page(APP_HEADER_ADDR, flash_buffer);
	HAL_FLASH_Lock();
}
