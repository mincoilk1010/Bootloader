# STM32 Bootloader & Vector Table Relocation Guide

Một hướng dẫn toàn diện và mã nguồn mẫu triển khai **Bootloader**, **Phân chia bộ nhớ (Memory Mapping)**, **Định vị lại bảng vector ngắt (VTOR)** và **Hàm nhảy (Jump Function)** cho dòng vi điều khiển STM32 (ARM Cortex-M3/M4).

---

## 📌 Nội Dung Chính (Table of Contents)

1. [Tổng Quan Kiến Trúc (Architecture Overview)](#1-tổng-quan-kiến-trúc)
2. [Bố Trí Bộ Nhớ Flash (Memory Mapping)](#2-bố-trí-bộ-nhớ-flash)
3. [Cấu Hình Linker Script (.ld)](#3-cấu-hình-linker-script)
4. [Định Vị Lại Bảng Vector Ngắt (VTOR)](#4-định-vị-lại-bảng-vector-ngắt)
5. [Cơ Chế & Mã Nguồn Hàm Nhảy (Jump Function)](#5-cơ-chế--mã-nguồn-hàm-nhảy)
6. [Quy Trình Kiểm Tra & Triển Khai](#6-quy-trình-kiểm-tra--triển-khai)

---

## 1. Tổng Quan Kiến Trúc

Khi khởi động dòng vi điều khiển ARM Cortex-M:
1. Lõi xử lý sẽ đọc giá trị **Main Stack Pointer (MSP)** ban đầu tại địa chỉ `0x08000000`.
2. Lõi xử lý đọc giá trị địa chỉ của **Reset Handler** tại `0x08000004` và bắt đầu thực thi chương trình Bootloader.

Hệ thống Bootloader này thực hiện nhiệm vụ:
- Kiểm tra tính hợp lệ của ứng dụng chính (**Application**) thông qua kiểm tra Checksum/CRC hoặc tín hiệu kích hoạt.
- Thực hiện chuyển đổi con trỏ Stack và địa chỉ thực thi từ Bootloader sang Application an toàn.

---

## 2. Bố Trí Bộ Nhớ Flash

Ví dụ phân chia bộ nhớ cho **STM32F103C8T6 (64KB Flash, 20KB RAM)**:

| Vùng Bộ Nhớ | Địa Chỉ Bắt Đầu | Địa Chỉ Kết Thúc | Kích Thước | Mục Đích Sử Dụng |
| :--- | :--- | :--- | :--- | :--- |
| **Bootloader** | `0x0800 0000` | `0x0800 3FFF` | 16 KB | Chương trình khởi động, nhận OTA/UART |
| **App Header** | `0x0800 4000` | `0x0800 43FF` | 1 KB | Metadata (Phiên bản, Kích thước, CRC32) |
| **Application** | `0x0800 4400` | `0x0800 FFFF` | 47 KB | Chương trình ứng dụng chính |

---

## 3. Cấu Hình Linker Script

Để chương trình Application hoạt động đúng tại địa chỉ `0x0800 4400`, tệp Linker Script (`.ld`) của project Application phải được tinh chỉnh:

### Bootloader Linker Script (`STM32F103C8TX_FLASH.ld`)
```ld
MEMORY
{
  RAM (xrw)   : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 16K
}
```

### Application Linker Script (`STM32F103C8TX_FLASH.ld`)
```ld
MEMORY
{
  RAM (xrw)        : ORIGIN = 0x20000000, LENGTH = 20K
  APP_HEADER (rx)  : ORIGIN = 0x08004000, LENGTH = 1K
  FLASH (rx)       : ORIGIN = 0x08004400, LENGTH = 47K
}
```

---

## 4. Định Vị Lại Bảng Vector Ngắt (VTOR)

Nếu không cài đặt lại **VTOR (Vector Table Offset Register)**, khi xảy ra ngắt (Interrupt) trong Application, MCU sẽ nhảy về bảng ngắt của Bootloader ở địa chỉ `0x08000000`, dẫn đến đơ/HardFault.

Trong mã nguồn **Application** (tại `main.c` hoặc `system_stm32f1xx.c`), thêm cấu hình VTOR ngay sau khi khởi động:

```c
#include "stm32f1xx.h"

#define FLASH_BASE          0x08000000UL
#define APPLICATION_OFFSET  0x00004400UL

void VectorTable_Init(void) {
    /* Đặt lại địa chỉ Bảng Vector Ngắt về vị trí ứng dụng */
    SCB->VTOR = FLASH_BASE | APPLICATION_OFFSET;
}
```

---

## 5. Cơ Chế & Mã Nguồn Hàm Nhảy

### Các bước bắt buộc trước khi nhảy sang Application:
1. **Tắt ngắt toàn cục** (`__disable_irq()`) và dừng SysTick.
2. **De-initialize ngoại vi** để trả RAM và thanh ghi ngoại vi về trạng thái mặc định.
3. Đọc địa chỉ **MSP** từ địa chỉ đầu tiên của App (`app_addr + 0`).
4. Đọc địa chỉ **Reset Handler** từ (`app_addr + 4`).
5. Gán giá trị MSP mới và thực hiện nhảy thông qua Con trỏ hàm (Function Pointer).

### Mã Nguồn Triển Khai (`bootloader.c`)

```c
#include "stm32f1xx.h"

typedef void (*pFunction)(void);

void Bootloader_JumpToApplication(uint32_t app_addr) {
    /* Kiểm tra địa chỉ Stack Pointer có hợp lệ trong SRAM không */
    if (((*(__IO uint32_t*)app_addr) & 0x2FFE0000) == 0x20000000) {
        
        /* 1. Tắt toàn bộ ngắt & SysTick */
        __disable_irq();
        SysTick->CTRL = 0;
        SysTick->LOAD = 0;
        SysTick->VAL  = 0;

        /* 2. Đưa các外圍 (Peripherals) về trạng thái reset */
        HAL_RCC_DeInit();
        HAL_DeInit();

        /* 3. Lấy địa chỉ Reset Handler của Application */
        uint32_t jump_addr = *(__IO uint32_t*)(app_addr + 4);
        pFunction jump_to_app = (pFunction)jump_addr;

        /* 4. Cài đặt lại Main Stack Pointer (MSP) */
        __set_MSP(*(__IO uint32_t*)app_addr);

        /* 5. Nhảy sang ứng dụng */
        jump_to_app();
    }
}
```

---

## 6. Quy Trình Kiểm Tra & Triển Khai

1. **Biên dịch Bootloader**: nạp vào STM32 từ địa chỉ `0x08000000`.
2. **Biên dịch Application**: đảm bảo đã chỉnh sửa File Linker `.ld` và cấu hình `SCB->VTOR`.
3. **Nạp Firmware**:
   - Sử dụng STM32CubeProgrammer để nạp file `.bin` / `.hex` của Application vào đúng địa chỉ `0x08004400`.
4. **Kiểm tra Log UART / Debug**:
   - Mở Serial Terminal xem Bootloader khởi chạy.
   - Bootloader gọi hàm nhảy `Bootloader_JumpToApplication(0x08004400)`.
   - Ứng dụng Application hoạt động bình thường và ngắt ngoại vi (Timer, UART, GPIO) phản hồi đúng.

---

## 📄 Giấy Phép (License)

Dự án này được cấp phép theo thư viện [MIT License](LICENSE).