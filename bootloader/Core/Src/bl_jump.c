/*
 * bl_jump.c
 *
 *  Created on: Sep 29, 2026
 *      Author: Admin
 */
#include "bl_jump.h"
#include "app_header.h"
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
	if((reset_handler & FF000000) != 0x08000000) {
		return 2;
	}

	//if a check pass return 0
	return 0;
}


