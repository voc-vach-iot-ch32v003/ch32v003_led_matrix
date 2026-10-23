# 📐 Ví dụ 02: Vẽ đồ họa và chọn góc xoay

## Mục tiêu

Vẽ khung viền, đường chéo (`ledMatrixDrawLine`) và chữ (`ledMatrixDrawString`), lần lượt với 4 góc xoay `LED_MATRIX_ROTATE_0 / 90 / 180 / 270`. Góc nào hiện tên đọc được đúng chiều là góc hợp với module bạn đang dùng.

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

- Mỗi góc xoay: khung viền quanh cả màn hình + dấu X trên khối đầu tiên (1.5 giây), sau đó tên góc `R0` / `R90` / `R180` / `R270` (2 giây).
- `90` / `270` xoay **từng khối** tại chỗ (cho module có khối hàn xoay), `180` lật **cả dải** (cho module lắp ngược).
- Chọn góc đọc đúng rồi gọi `ledMatrixSetRotation()` một lần trong `setup()` của chương trình thật.
