# 📜 Ví dụ 01: Cuộn chữ

## Mục tiêu

Cuộn chuỗi `Hello CH32V003!` từ phải sang trái trên module ma trận LED 4 khối (FC-16), rồi cuộn chữ `IoT` lên và xuống. Mỗi lần gọi `ledMatrixScrollTextAnimation()` dịch chữ 1 pixel trong Frame Buffer, `ledMatrixUpdate()` đẩy cả màn hình ra chỉ bằng 8 khung SPI.

## Sơ đồ nối dây (Pinout)

`MCU_PIN<n>` là số chân vật lý trên vỏ chip, nên chân port tương ứng khác nhau theo gói vỏ:

| Tín hiệu | Hằng số | J4M6 (SOP8) | A4M6 (SOP16) | F4P6 (TSSOP20) | Ghi chú |
| --- | --- | --- | --- | --- | --- |
| CS / LOAD | `MATRIX_CS_PIN` (`MCU_PIN3`) | PA2 | PC3 | PD6 | |
| CLK | `MATRIX_CLK_PIN` (`MCU_PIN5`) | PC1 | PC6 | PA1 | |
| DIN | `MATRIX_DIN_PIN` (`MCU_PIN6`) | PC2 | PC7 | PA2 | Vào DIN của khối đầu chuỗi |
| VCC / GND | | | | | Module cấp 5V, GND chung với chip |

> ⚠️ MAX7219 cấp 5V yêu cầu mức HIGH tối thiểu 3.5V. Chip chạy 3.3V thường vẫn điều khiển được, nhưng nằm ngoài thông số. Chắc chắn nhất là cấp **5V cho cả CH32V003**.

## Build & Nạp

```bash
pio run -e CH32V003J4M6 -t upload   # hoặc CH32V003A4M6 / CH32V003F4P6
```

## Kết quả mong đợi

- Chuỗi `Hello CH32V003!` chạy từ mép phải sang mép trái 2 lượt (mỗi pixel 30ms).
- Chữ `IoT` đi từ dưới lên rồi từ trên xuống.
- Chữ bị ngược hoặc xoay 90°: do module lắp LED theo hướng khác, gọi `ledMatrixSetRotation()` (xem ví dụ 02).
