/*
 * type_config.h
 *
 *  Created on: Oct 5, 2026
 *      Author: Admin
 */

#ifndef INC_TYPE_CONFIG_H_
#define INC_TYPE_CONFIG_H_

#include "stdint.h"

typedef uint16_t u16;
typedef uint8_t u8;
typedef uint32_t u32;

typedef int		i32;

/* khung giao thuc : A5 |CMD|len|playload|crc16
 *
 * playload {
 * 	size
 * 	crc32
 * 	version
 * }
 * or{
 * 	len = 4 byte offset + data;
 * 	// offet : vi tri cua firmware hien tai
 * 	--> dia chi ghi = APP_ADDR + offset
 * }
 */

/* Khung giao thuc, lenh, ma trang thai: xem protocol_frame.h */
#define BOOT_WAIT_MS 3000U

/* OTA confirm + rollback (BL_FLAG dinh nghia trong bl_flag.h, include o tren) */
#define OTA_PENDING       0xA5A5A5A5U    /* vua nap, chua tu xac nhan chay on */
#define OTA_CONFIRMED     0x00000000U    /* app da tu ghi, bao chay on dinh */
#define MAX_BOOT_ATTEMPTS 3U             /* so lan tu nhay toi da truoc khi rollback */


#endif /* INC_TYPE_CONFIG_H_ */
