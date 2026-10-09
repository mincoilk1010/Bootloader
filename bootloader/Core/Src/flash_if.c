/*
 * flash_if.c
 *
 *  Created on: Oct 9, 2026
 *      Author: DELL
 */

#include "flash_if.h"

/* Xóa các Page Flash thuộc vùng chứa App */
uint8_t Flash_EraseAppSpace(uint32_t app_size)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError;

    HAL_FLASH_Unlock();

    // Tính số lượng Page cần xóa
    uint32_t pageCount = (app_size + FLASH_PAGE_SIZE_F1 - 1) / FLASH_PAGE_SIZE_F1;
    if (pageCount == 0) pageCount = 48; // Mặc định xóa hết 48KB còn lại của Flash C8T6

    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = APP_START_ADDRESS;
    EraseInitStruct.NbPages     = pageCount;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return 0; // Lỗi xóa
    }

    HAL_FLASH_Lock();
    return 1; // Xóa thành công
}

/* Ghi dữ liệu dạng Half-Word (16-bit) cho STM32F1 */
uint8_t Flash_WriteData(uint32_t address, uint8_t *pData, uint32_t length)
{
    HAL_FLASH_Unlock();

    for (uint32_t i = 0; i < length; i += 2)
    {
        uint16_t halfWordData = pData[i] | (pData[i + 1] << 8);
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address + i, halfWordData) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return 0; // Lỗi ghi
        }
    }

    HAL_FLASH_Lock();
    return 1; // Ghi thành công
}
