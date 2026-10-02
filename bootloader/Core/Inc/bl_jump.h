/*
 * bl_jump.h
 *
 *  Created on: Sep 29, 2026
 *      Author: Admin
 */

#ifndef INC_BL_JUMP_H_
#define INC_BL_JUMP_H_

#include "stdint.h"
#include "flash_layout.h"
#include "stm32f1xx_hal.h"
#include "core_cm3.h"
typedef void (*pFunction)(void);

void JumptoApplication(void);
int bootloader_is_app_valid(void);
#endif /* INC_BL_JUMP_H_ */
