/*
 * flash.h
 *
 *  Created on: Sep 22, 2026
 *      Author: Admin
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_
#include "stm32f103xx.h"
void Flash_Unlock();
void Flash_lock();
void Flash_Erase_Sector(char sector);
void Flash_Program(u8 *addr, u8 value );
#endif /* INC_FLASH_H_ */
