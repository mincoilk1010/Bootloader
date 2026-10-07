import os
import struct
import serial
import crc32_stm32

SOF = 0xAA
CMD_START, CMD_DATA, CMD_END = 0x01, 0x02, 0x03
ACK, NACK = 0x06, 0x15
PAYLOAD = 128            # khớp kích thước bộ đệm của bootloader
MAX_RETRY = 3


def frame(cmd: int, data: bytes = b"") -> bytes:
    body = bytes([cmd]) + struct.pack("<H", len(data)) + data
    return bytes([SOF]) + body + bytes([sum(body) & 0xFF])


def send_and_wait(ser, pkt: bytes, timeout: float) -> int:
    ser.timeout = timeout
    ser.reset_input_buffer()
    ser.write(pkt)
    r = ser.read(1)
    return r[0] if r else -1


def flash(port, baud, path, corrupt, on_progress, on_log, stop_event):
    """Nạp một file .bin. Trả về True nếu thành công."""
    with open(path, "rb") as f:
        image = f.read()
    if not image:
        on_log("File rỗng!")
        return False

    crc = crc32_stm32(image)                 # CRC của ảnh gốc
    if corrupt:                              # làm hỏng 1 byte, giữ CRC gốc
        b = bytearray(image)
        b[len(b) // 2] ^= 0xFF
        image = bytes(b)
        on_log("[Chế độ thử lỗi] đã đổi 1 byte của ảnh")

    packets = [("START", frame(CMD_START, struct.pack("<II", len(image), crc)))]
    for off in range(0, len(image), PAYLOAD):
        packets.append(("DATA", frame(CMD_DATA, struct.pack("<I", off) + image[off:off + PAYLOAD])))
    packets.append(("END", frame(CMD_END)))

    on_log(f"File: {os.path.basename(path)} - {len(image)} byte - CRC32 = 0x{crc:08X}")
    on_log(f"Tổng {len(packets)} gói, mở {port} @ {baud}")

    try:
        ser = serial.Serial(port, baud, timeout=1)
    except serial.SerialException as e:
        on_log(f"Không mở được cổng: {e}")
        return False

    with ser:
        for i, (name, pkt) in enumerate(packets):
            if stop_event.is_set():
                on_log("Đã hủy bởi người dùng.")
                return False
            tmo = 8.0 if name == "START" else 1.0     # START chờ lâu vì xóa Flash
            resp = -1
            for _ in range(MAX_RETRY + 1):
                resp = send_and_wait(ser, pkt, tmo)
                if resp == ACK:
                    break
                if name == "END" and resp == NACK:
                    on_log("LỖI: CRC toàn ảnh sai -> bootloader từ chối, ở lại bootloader.")
                    return False
            else:
                why = "timeout" if resp == -1 else "NACK"
                on_log(f"LỖI: gói {name} #{i} thất bại ({why}) sau {MAX_RETRY} lần gửi lại.")
                return False
            on_progress(int(100 * (i + 1) / len(packets)))
    on_log("Nạp xong, CRC đúng. Bootloader nhảy sang ứng dụng.")
    return True