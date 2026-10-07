/*
 * flash_layout.h
 *
 *  Created on: Sep 29, 2026
 *      Author: Admin
 */

#ifndef INC_FLASH_LAYOUT_H_
#define INC_FLASH_LAYOUT_H_

#define BL_START_ADDR 	0x08000000  // 16Kb
#define APP_HEADER_ADDR 	0X08004000 //1Kb
#define APP_START_ADDR      0x08004400  //47Kb
#define APP_SIZE_MAX 47*1024;

#endif /* INC_FLASH_LAYOUT_H_ */
