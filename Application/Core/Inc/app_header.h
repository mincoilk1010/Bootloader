/*
 * app_header.h
 *
 *  Created on: Oct 1, 2026
 *      Author: minh-bui
 */

#ifndef INC_APP_HEADER_H_
#define INC_APP_HEADER_H_

#include "flash_layout.h"
#define APP_MAGIC 0xFFABABFF

typedef struct
{
    uint32_t magic;
    uint32_t size; //size in bytes
    uint32_t crc; //CRC32 of application
    uint32_t version;
} app_header_t;


#endif /* INC_APP_HEADER_H_ */
