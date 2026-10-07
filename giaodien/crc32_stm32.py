import struct
import sys


def crc32_stm32(data: bytes) -> int:
    """CRC phần cứng STM32: poly 0x04C11DB7, init 0xFFFFFFFF, theo từ 32-bit LE."""
    if len(data) % 4:
        data += b"\xFF" * (4 - len(data) % 4)
    crc = 0xFFFFFFFF
    for (word,) in struct.iter_unpack("<I", data):
        crc ^= word
        for _ in range(32):
            crc = ((crc << 1) ^ 0x04C11DB7) & 0xFFFFFFFF if crc & 0x80000000 \
                  else (crc << 1) & 0xFFFFFFFF
    return crc

if __name__ == "__main__":
    # Chạy riêng để kiểm tra:  py crc_calc.py appA.bin
    if len(sys.argv) != 2:
        sys.exit("Cách dùng: py crc_calc.py <file.bin>")
    with open(sys.argv[1], "rb") as f:
        d = f.read()
    print(f"Size       = {len(d)} bytes")
    print(f"CRC stm32  = 0x{crc32_stm32(d):08X}")