/*
 * flash_prog.c
 *
 *  Created on: Oct 5, 2026
 *      Author: Admin
 */
#include "flash_prog.h"
#include "flash_layout.h"

int flash_erase(u32 addr, u32 pages)
{
    if (addr < APP_HEADER_ADDR || (addr & (PAGE_SZ - 1)) || addr + pages * PAGE_SZ > FLASH_LIMIT) return -1;
    /* STM32F1 KHONG co truong Banks (chi F4/F7/H7 moi co) - dung ten truong
     * (designated initializer) de khong le thuoc dung thu tu struct. */
    FLASH_EraseInitTypeDef e = { .TypeErase = FLASH_TYPEERASE_PAGES, .PageAddress = addr, .NbPages = pages };
    uint32_t err; int ok;
    HAL_FLASH_Unlock();
    ok = (HAL_FLASHEx_Erase(&e, &err) == HAL_OK);
    HAL_FLASH_Lock();
    return ok ? 0 : -1;

}
int flash_write(uint32_t addr, const uint8_t *d, uint32_t len)
{

    if (addr < APP_HEADER_ADDR || (addr & 3) || (len & 3) || addr + len > FLASH_LIMIT) return -1;
    HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < len; i += 4) {
        uint32_t w = d[i] | (d[i+1] << 8) | (d[i+2] << 16) | ((uint32_t)d[i+3] << 24);
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i, w) != HAL_OK ||
            *(volatile uint32_t *)(addr + i) != w) { HAL_FLASH_Lock(); return -1; }
    }
    HAL_FLASH_Lock();
    return 0;
}
