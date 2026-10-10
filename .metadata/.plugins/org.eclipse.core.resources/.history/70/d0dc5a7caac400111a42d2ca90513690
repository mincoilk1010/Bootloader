/*
 * app_ota.c
 *
 *  Created on: Oct 5, 2026
 *      Author: Admin
 */


#include "app_ota.h"
#include "bl_flag.h"

void enable_ota_request(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_BKP_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    BKP->DR1 = BL_FLAG;
    NVIC_SystemReset();
}

