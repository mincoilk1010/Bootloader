/*
 * bl_jump.c
 *
 *  Created on: Sep 29, 2026
 *      Author: Admin
 */
#include "bl_jump.h"
#include "app_header.h"
#include "crc_cal.h"
typedef void (*pFunction)(void);

void JumptoApplication(void)
{
	uint32_t appStack;
	uint32_t appResetHandler;
	pFunction appEntry;

	appStack = *(volatile uint32_t*)APP_START_ADDR;
	appResetHandler = *(volatile uint32_t*)(APP_START_ADDR + 4);
	appEntry = (pFunction)appResetHandler;

	__disable_irq();

	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	__set_MSP(appStack);

	appEntry();
}

int bootloader_is_app_valid(void) {
	uint32_t HDR_ADDR = APP_HEADER_ADDR;
	const app_header_t *app_hdr = (const app_header_t *)HDR_ADDR;

	//magic number validation
	if(app_hdr->magic != APP_MAGIC) {
		return 1;
	}

	//reset handle check
	uint32_t reset_handler = *(uint32_t *)(APP_START_ADDR + 4);
	if((reset_handler & 0xFF000000) != 0x08000000) {
		return 2;
	}

	//app size check
	if (app_hdr->size == 0 || app_hdr->size > APP_MAX_SIZE) {
	    return 3;
	}

	//crc check
	uint32_t calculated_crc = cal_hw_crc32((uint32_t*)APP_START_ADDR, app_hdr->size / 4);
	if(calculated_crc != app_hdr->crc) {
		return 4;
	}
	//if a check pass return 0
	return 0;
}


