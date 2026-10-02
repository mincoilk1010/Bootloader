/*
 * flash.c
 *
 *  Created on: Sep 22, 2026
 *      Author: Admin
 */

#include "flash.h"
void Flash_Unlock()
{
	if((Flash->CR)&(1 << 7))
	{
		Flash->KEYR = 0x45670123;
		Flash->KEYR = 0xCDEF89AB;
	}
}
void Flash_lock()
{
	Flash->CR |= 1 << 7;
}

void Flash_Erase_Page(u32 addr)  // addr địa chỉ của page cần xóa
{
	Flash_Unlock();
	while((Flash -> SR) & (1 << 0));
	Flash->CR |= 1 << 1;
	Flash->AR = addr;
	Flash->CR |= 1 << 6; // BẮT ĐẦU XÓA

}
void Flash_Program(u8 *addr, u8 value );
