/*
 * flash_operations.c
 *
 *  Created on: Oct 9, 2026
 *      Author: minh-bui
 */

#include "flash_operations.h"
#include "main.h"

void flash_read_page(uint32_t page_start_addr, uint32_t *buffer) {
	uint32_t *flash_ptr = (uint32_t *) page_start_addr;
	for(uint32_t i = 0; i < 5; i++) {
		buffer[i] = flash_ptr[i];
	}
}

//1 page = 1KB -> chay ham` nay voi page_addr = 0x0800 4000 se xoa nguyen phan app header
void flash_erase_page (uint32_t page_addr) {
	FLASH_EraseInitTypeDef erase;
	uint32_t error;

	erase.TypeErase = FLASH_TYPEERASE_PAGES;
	erase.PageAddress = page_addr;
	erase.NbPages = 1;

	HAL_FLASHEx_Erase(&erase, &error);
}

void flash_write_page(uint32_t page_start_addr, uint32_t *buffer) {
	uint32_t addr = page_start_addr;
	for(uint32_t i = 0; i < 5; i++) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, buffer[i]);
		addr += 4;
	}
}
