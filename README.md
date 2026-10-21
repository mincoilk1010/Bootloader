# STM32F103 Bootloader – App Header, Hardware CRC32 & Flash Operations

Bootloader sử dụng cho **STM32F103C8T6** bằng **STM32CubeIDE + HAL**. Sau mỗi lần Reset, **Bootloader kiểm tra tính hợp lệ của application** (magic, reset vector, size, CRC32) rồi mới nhảy vào app. Nếu app không hợp lệ, bootloader báo lỗi qua UART và dừng lại, không chạy firmware hỏng.

| Hạng mục | Mô tả | File chính |
|---|---|---|
| **App header** | Cấu trúc metadata đặt ở một page flash riêng, dùng để xác thực app | `app_header.h`, `flash_layout.h`, linker script |
| **Kiểm tra app** | 4 bước kiểm tra trước khi jump, trả về mã lỗi riêng cho từng bước | `bl_jump.c` |
| **CRC32 bằng hardware** | Dùng peripheral CRC tích hợp của STM32 thay vì tính bằng phần mềm | `crc_cal.c` |
| **Flash read / erase / write** | Hàm thao tác flash dùng chung cho bootloader và application | `flash_operations.c` |
| **Cờ OTA** | App set cờ trong header rồi reset, bootloader đọc cờ và xóa cờ | `app_ota.c`, `bl_ota.c` |

---

## 1. Cấu trúc thư mục

```
.
├── bootloader/                  # Project bootloader (chạy tại 0x08000000)
│   ├── STM32F103C8TX_FLASH.ld
│   └── Core/
│       ├── Inc/  app_header.h  bl_jump.h  bl_ota.h  crc_cal.h
│       │         flash_layout.h  flash_operations.h
│       └── Src/  main.c  bl_jump.c  bl_ota.c  crc_cal.c  flash_operations.c
└── Application/                 # Project application (chạy tại 0x08004400)
    ├── STM32F103C8TX_FLASH.ld
    └── Core/
        ├── Inc/  app_header.h  app_ota.h  flash_layout.h  flash_operations.h
        └── Src/  main.c  app_ota.c  flash_operations.c
```

---

## 2. Bản đồ flash (Flash layout)

STM32F103C8T6 có 64 KB flash, mỗi page 1 KB. Flash được chia thành 3 vùng:

```
0x0800_0000  ┌──────────────────────────┐
             │  Bootloader   (16 KB)    │  BL_START_ADDR
0x0800_4000  ├──────────────────────────┤
             │  App Header   ( 1 KB)    │  APP_HEADER_ADDR  (đúng 1 page)
0x0800_4400  ├──────────────────────────┤
             │  Application  (47 KB)    │  APP_START_ADDR
0x0800_FFFF  └──────────────────────────┘
```

`flash_layout.h`:

```c
#define BL_START_ADDR     0x08000000   // 16 KB
#define APP_HEADER_ADDR   0x08004000   // 1 KB
#define APP_START_ADDR    0x08004400   // 47 KB
#define APP_MAX_SIZE      47*1024
```

Header được đặt riêng một page 1 KB để có thể **xóa và ghi lại độc lập** với code của application (flash chỉ xóa được theo page).

---

## 3. App Header

```c
#define APP_MAGIC 0xFFABABFF

typedef struct
{
    uint32_t OTA;      // cờ yêu cầu OTA (1 = có yêu cầu)
    uint32_t magic;    // APP_MAGIC, nhận diện header hợp lệ
    uint32_t size;     // kích thước application (bytes)
    uint32_t crc;      // CRC32 của application
    uint32_t version;  // phiên bản firmware
} app_header_t;        // 5 word = 20 bytes
```

`size` và `crc` chỉ biết được **sau khi build xong** (xem mục 5).

---

## 4. Kiểm tra application trước khi jump

Hàm `bootloader_is_app_valid()` trong `bl_jump.c` thực hiện 4 bước kiểm tra theo thứ tự, trả về `0` nếu hợp lệ, ngược lại trả về mã lỗi:

| Mã | Bước kiểm tra | Điều kiện lỗi | Thông báo UART |
|---|---|---|---|
| `1` | **Magic number** | `app_hdr->magic != APP_MAGIC` | `MAGIC ERROR!!` |
| `2` | **Reset handler** | Địa chỉ reset vector (tại `APP_START_ADDR + 4`) không nằm trong vùng flash `0x08xxxxxx` | `RESET ERROR!!` |
| `3` | **Kích thước** | `size == 0` hoặc `size > APP_MAX_SIZE` | `SIZE ERROR!!` |
| `4` | **CRC32** | CRC tính được ≠ `app_hdr->crc` | `CRC ERROR!!` |

```c
int bootloader_is_app_valid(void)
{
    const app_header_t *app_hdr = (const app_header_t *)APP_HEADER_ADDR;

    if (app_hdr->magic != APP_MAGIC)
        return 1;

    uint32_t reset_handler = *(uint32_t *)(APP_START_ADDR + 4);
    if ((reset_handler & 0xFF000000) != 0x08000000)
        return 2;

    if (app_hdr->size == 0 || app_hdr->size > APP_MAX_SIZE)
        return 3;

    uint32_t calculated_crc =
        cal_hw_crc32((uint32_t *)APP_START_ADDR, app_hdr->size / 4);
    if (calculated_crc != app_hdr->crc)
        return 4;

    return 0;
}
```

## 5. Tính CRC32 bằng hardware

STM32 có sẵn peripheral **CRC calculation unit**, tính CRC nhanh hơn software và không tốn bảng lookup trong flash (quan trọng khi bootloader chỉ có 16 KB).

```c
uint32_t cal_hw_crc32(uint32_t *data, uint32_t length_in_words)
{
    CRC->CR = CRC_CR_RESET;               // reset giá trị về 0xFFFFFFFF

    for (uint32_t i = 0; i < length_in_words; i++)
        CRC->DR = data[i];                // đẩy từng word 32-bit vào

    return CRC->DR;                       // đọc kết quả
}
```

### Thông số thuật toán

CRC hardware của STM32F1 là **CRC-32/MPEG-2**, khác với CRC-32 thông dụng (zlib/Ethernet):

| Tham số | Giá trị |
|---|---|
| Polynomial | `0x04C11DB7` |
| Init | `0xFFFFFFFF` |
| Độ rộng dữ liệu | **32-bit word** (không hỗ trợ byte) |
| Reflect input / output | Không |
| XOR out | `0x00000000` |

> Vì hardware xử lý theo word nên `length_in_words = size / 4`. Kích thước app phải là **bội số của 4**; nếu không, 1–3 byte cuối sẽ bị bỏ qua. Khi tính CRC ở máy tính, cần pad file `.bin` bằng `0xFF` cho đủ bội số 4 và dùng **cùng thuật toán** (không dùng `zlib.crc32` / `binascii.crc32` vì sẽ ra kết quả khác).

### Tính CRC phía máy tính để điền vào header

Ví dụ script Python tính cùng giá trị với hardware (đọc từng word little-endian):

```python
import struct, sys

def crc32_stm32(data: bytes) -> int:
    if len(data) % 4:
        data += b"\xFF" * (4 - len(data) % 4)   # pad cho đủ word
    crc = 0xFFFFFFFF
    for (word,) in struct.iter_unpack("<I", data):
        crc ^= word
        for _ in range(32):
            crc = ((crc << 1) ^ 0x04C11DB7) if crc & 0x80000000 else (crc << 1)
            crc &= 0xFFFFFFFF
    return crc

if __name__ == "__main__":
    raw = open(sys.argv[1], "rb").read()
    print(f"size = {len(raw)}  (padded: {(len(raw)+3)//4*4})")
    print(f"crc  = 0x{crc32_stm32(raw):08X}")
```

Lưu ý: file `.bin` đưa vào phải chỉ chứa **vùng application** bắt đầu từ `0x08004400` (không gồm header ở `0x08004000`), vì bootloader tính CRC từ `APP_START_ADDR`.

---

## 6. Flash read / erase / write

Các hàm trong `flash_operations.c` bọc lại HAL flash driver, dùng được ở cả bootloader và application:

```c
void flash_read_page (uint32_t page_start_addr, uint32_t *buffer);  // đọc 5 word
void flash_erase_page(uint32_t page_addr);                          // xóa 1 page (1 KB)
void flash_write_page(uint32_t page_start_addr, uint32_t *buffer);  // ghi 5 word
```

| Hàm | Cách hoạt động |
|---|---|
| `flash_read_page` | Ép địa chỉ về `uint32_t *` và copy 5 word (đúng kích thước `app_header_t`) vào buffer |
| `flash_erase_page` | Dùng `HAL_FLASHEx_Erase` với `FLASH_TYPEERASE_PAGES`, `NbPages = 1`. Gọi với `0x08004000` sẽ xóa toàn bộ vùng app header |
| `flash_write_page` | Ghi lần lượt từng word bằng `HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, ...)`, địa chỉ tăng 4 mỗi lần |

### Quy trình cập nhật header: read → modify → erase → write

Flash STM32 **không ghi đè trực tiếp được**: một ô nhớ phải được xóa (về `0xFFFFFFFF`) trước khi ghi lại. Vì vậy muốn sửa một field trong header phải đọc cả header ra RAM trước:

```c
HAL_FLASH_Unlock();
flash_read_page (APP_HEADER_ADDR, flash_buffer);   // 1. đọc cả header ra RAM
flash_buffer[0] = OTA_FLAG_START;                  // 2. sửa field cần đổi
flash_erase_page(APP_HEADER_ADDR);                 // 3. xóa page
flash_write_page(APP_HEADER_ADDR, flash_buffer);   // 4. ghi lại toàn bộ header
HAL_FLASH_Lock();
```

---

## 7. Cờ OTA (OTA flag)

Trường `OTA` của header dùng để application "nhắn" cho bootloader.

**Phía application** (`app_ota.c`): đặt `OTA = 1` bằng quy trình read–modify–erase–write ở trên, rồi `NVIC_SystemReset()`:

```c
void enable_ota_request(void)
{
    HAL_FLASH_Unlock();
    flash_read_page(APP_HEADER_ADDR, flash_buffer);
    flash_buffer[0] = OTA_FLAG_START;      // 1
    flash_erase_page(APP_HEADER_ADDR);
    flash_write_page(APP_HEADER_ADDR, flash_buffer);
    HAL_FLASH_Lock();
    NVIC_SystemReset();
}
```

**Phía bootloader** (`bl_ota.c`): `check_ota_request()` trả về `0` nếu cờ bằng `1`; `clear_ota_flag()` ghi `OTA = 0` bằng cùng quy trình để không bị lặp lại ở lần boot sau.

---

## 8. Luồng hoạt động của bootloader

```
                 Reset
                   │
                   ▼
        Khởi tạo HAL, clock, GPIO, UART
        In "Inside Bootloader!!"
                   │
                   ▼
         ┌─ OTA flag == 1 ? ─┐
         │ có                │ không
         ▼                   ▼
  In "OTA Requested"   bootloader_is_app_valid()
  clear_ota_flag()           │
  Nháy LED PA4          ┌────┴─────┐
  (500 ms)              │ == 0     │ != 0 (mã lỗi 1–4)
                        ▼          ▼
                JumptoApplication  In lỗi qua UART
                                   Nháy LED PA4 nhanh (100 ms)
```

- LED PA4 nháy **500 ms** = đang ở chế độ OTA.
- LED PA4 nháy **100 ms** = app không hợp lệ (xem UART để biết lỗi nào).

---

## 9. Phần cứng & công cụ

- **MCU:** STM32F103C8T6 (LQFP48), 64 KB flash / 20 KB RAM
- **IDE:** STM32CubeIDE, STM32Cube FW_F1 V1.8.7 (HAL), GCC ARM
- **UART debug:** USART1 (PA9 = TX, PA10 = RX)
- **LED báo trạng thái:** PA4
- **Nạp / debug:** ST-Link qua SWD (PA13 / PA14)

## 10. Cách build & chạy

1. Import cả hai project `bootloader` và `Application` vào STM32CubeIDE.
2. Build và nạp **bootloader** vào `0x08000000`.
3. Build **Application**, lấy file `.bin`, tính `size` và `crc` bằng script ở mục 5.
4. Điền `size`, `crc` (và `version`) vào `app_header` rồi build lại / nạp lại. Application phải được nạp ở `0x08004400`, header ở `0x08004000`.
5. Mở serial terminal (115200 hoặc theo cấu hình trong `usart.c`) và reset board để xem log của bootloader.

---
