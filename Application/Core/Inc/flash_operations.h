/*
 * flash_operations.h
 *
 *  Created on: Oct 9, 2026
 *      Author: minh-bui
 */

#ifndef INC_FLASH_OPERATIONS_H_
#define INC_FLASH_OPERATIONS_H_
#include <stdint.h>

void flash_read_page(uint32_t app_hdr_start_addr, uint32_t *buffer);
void flash_erase_page (uint32_t page_addr);
void flash_write_page(uint32_t page_start_addr, uint32_t *buffer);

#endif /* INC_FLASH_OPERATIONS_H_ */
