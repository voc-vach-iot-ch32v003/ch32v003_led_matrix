# 🧱 ch32v003_led_matrix

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Supported-orange.svg)
![Framework](https://img.shields.io/badge/Framework-ch32v003fun-green.svg)
![MCU](https://img.shields.io/badge/MCU-CH32V003-red.svg)

Thư viện đồ họa cho **LED Matrix 8x8** ghép nối tiếp qua IC MAX7219 (VD: module FC-16 4 khối) trên vi điều khiển CH32V003. Có Frame Buffer, điểm / đường thẳng / chữ font 5x7, xoay màn hình và cuộn chữ 4 hướng. Chạy được trên cả 3 gói vỏ J4M6, A4M6, F4P6 bằng SPI phần mềm, hoặc SPI1 phần cứng trên F4P6.

---

## 🚀 Tính năng nổi bật

- **Cập nhật nhanh:** `ledMatrixUpdate()` đẩy cả màn hình N khối chỉ bằng **8 khung SPI** (1 khung / hàng cho cả chuỗi), không phải 8 × N.
- **Vẽ trong Frame Buffer:** `ledMatrixDrawPixel`, `ledMatrixDrawLine` (Bresenham), `ledMatrixDrawChar` / `ledMatrixDrawString` (font ASCII 5x7, 32–126). Điểm ngoài màn hình (kể cả tọa độ âm) được bỏ qua, không ghi tràn bộ đệm, nên vẽ được chữ ló dần từ mép.
- **Xoay màn hình:** `90` / `270` xoay **từng khối** tại chỗ (module có khối hàn xoay), `180` lật **cả dải** (module lắp ngược). Tọa độ vẽ luôn là `(số khối × 8) × 8`.
- **Cuộn chữ 4 hướng:** trái, phải, lên, xuống. Mỗi lần gọi `ledMatrixScrollTextAnimation()` dịch chữ 1 pixel.
- **Nhiều màn hình độc lập:** mọi trạng thái (bộ đệm, góc xoay, vị trí cuộn) nằm trong Handle. Nhiều màn hình dùng chung bus SPI, mỗi màn một chân CS.

---

## 📦 Cài đặt (Installation)

```ini
lib_deps =
    https://github.com/voc-vach-iot-ch32v003/ch32v003_led_matrix.git#v2.0.0
```

Thư viện tự kéo theo `ch32v003_max7219`, `ch32v003_spi`, `ch32v003_gpio` (phiên bản đã ghim trong `library.json`).

---

## ⚙️ Cấu hình môi trường

```ini
build_flags = -DCH32V003_J4M6 ; hoặc -DCH32V003_A4M6 / -DCH32V003_F4P6
```

Thư viện không có macro cấu hình. Hằng dùng khi tính kích thước:

| Hằng | Giá trị | Ý nghĩa |
| :-- | :-- | :-- |
| `LED_MATRIX_MODULE_SIZE` | `8` | Frame Buffer cần `số khối × LED_MATRIX_MODULE_SIZE` byte |
| `LED_MATRIX_CHAR_WIDTH` | `6` | Mỗi ký tự rộng 6 pixel (5 + 1 cột cách): chuỗi n ký tự rộng `n × 6` |

> ⚠️ **Mức logic:** MAX7219 cấp 5V yêu cầu mức HIGH tối thiểu 3.5V. Chip chạy 3.3V thường vẫn điều khiển được, nhưng nằm ngoài thông số. Chắc chắn nhất là cấp **5V cho cả CH32V003**.
>
> ⚠️ **Hướng LED:** mỗi hãng lắp LED theo hướng khác nhau. Chữ bị ngược hoặc xoay thì thử `ledMatrixSetRotation()` với 4 góc (ví dụ `02_graphics_rotation`).

---

## 📝 Code mẫu sử dụng (Quick Start)

Cuộn chữ trên module FC-16 4 khối, SPI phần mềm nên chạy được trên cả 3 gói vỏ:

```c
#include "ch32fun.h"
#include "ch32v003_delay.h"
#include "ch32v003_gpio.h"
#include "ch32v003_led_matrix.h"
#include "ch32v003_spi.h"

#define MATRIX_COUNT 4     // 4 khối 8x8
#define CS_PIN  MCU_PIN3   // Chân vật lý số 3 (J4M6: PA2, A4M6: PC3, F4P6: PD6)
#define CLK_PIN MCU_PIN5   // Chân vật lý số 5 (J4M6: PC1, A4M6: PC6, F4P6: PA1)
#define DIN_PIN MCU_PIN6   // Chân vật lý số 6 (J4M6: PC2, A4M6: PC7, F4P6: PA2)

static SPI_DriverHandle_t spiBus = {
    .driverMode = SPI_MODE_SOFTWARE,
    .config.software = {.sckPin = CLK_PIN, .mosiPin = DIN_PIN, .misoPin = MCU_PIN_NONE},
};

static LED_MATRIX_DriverHandle_t matrix;
static uint8_t frameBuffer[MATRIX_COUNT * LED_MATRIX_MODULE_SIZE];

static void setup(void)
{
    spiInit(&spiBus); // Khởi tạo bus trước
    ledMatrixInit(&matrix, &spiBus, CS_PIN, frameBuffer, MATRIX_COUNT);
    ledMatrixSetBrightness(&matrix, 2);
    ledMatrixSetScrollDirection(&matrix, LED_MATRIX_SCROLL_LEFT);
}

static void loop(void)
{
    ledMatrixScrollTextAnimation(&matrix, "Hello CH32V003!", 0, true); // Dịch 1 pixel trong Frame Buffer
    ledMatrixUpdate(&matrix);                                           // Đẩy ra màn hình (8 khung SPI)
    delayMs(30);
}

int main(void)
{
    SystemInit();
    delayMs(1000); // Khoảng chờ an toàn tránh brick chip

    setup();
    while (1)
    {
        loop();
    }
}
```

Vẽ tĩnh:

```c
ledMatrixClearBuffer(&matrix);
ledMatrixDrawLine(&matrix, 0, 7, 7, 0, true);       // Đường chéo trên khối đầu tiên
ledMatrixDrawString(&matrix, 9, 0, "OK", true);     // Chữ bắt đầu ở cột 9
ledMatrixDrawPixel(&matrix, 31, 7, true);           // Góc dưới phải (4 khối)
ledMatrixUpdate(&matrix);
```

Dùng SPI1 phần cứng trên F4P6 (SCK = PC5, MOSI = PC6) thì chỉ cần đổi Handle của bus:

```c
static SPI_DriverHandle_t spiBus = {
    .driverMode = SPI_MODE_HARDWARE,
    .config.hardware = {.clockDivider = SPI_CLOCK_DIVIDER_8}, // 6MHz (MAX7219 tối đa 10MHz)
};
```

---

## 📁 Danh sách Ví dụ mẫu (Examples)

| Ví dụ | Mô tả |
| :-- | :-- |
| [`01_scroll_text`](examples/01_scroll_text/) | Cuộn chuỗi sang trái 2 lượt, rồi cuộn chữ lên / xuống. |
| [`02_graphics_rotation`](examples/02_graphics_rotation/) | Vẽ khung viền, đường chéo, chữ với 4 góc xoay để chọn góc hợp với module. |

---

## 📑 Tra cứu API (API Reference)

### Hàm

| Hàm xử lý | Tham số | Giá trị trả về | Mô tả |
| :-- | :-- | :-- | :-- |
| `ledMatrixInit(dev, spiHandle, csPin, buffer, matrixCount)` | `LED_MATRIX_DriverHandle_t*`, `const SPI_DriverHandle_t*`, `uint8_t`, `uint8_t*`, `uint8_t` | `uint8_t` | Gắn bus + chân CS + bộ đệm (`matrixCount × 8` byte), khởi tạo chuỗi MAX7219, xóa bộ đệm và màn hình. Trả `0` nếu Handle / bộ đệm / bus `NULL` hoặc `matrixCount = 0`. Bus phải được `spiInit()` trước. |
| `ledMatrixClearBuffer(dev)` | `LED_MATRIX_DriverHandle_t*` | `void` | Xóa Frame Buffer (chưa đẩy ra màn hình). |
| `ledMatrixUpdate(dev)` | `const LED_MATRIX_DriverHandle_t*` | `void` | Đẩy Frame Buffer ra màn hình (8 khung SPI). |
| `ledMatrixSetRotation(dev, rotation)` | `LED_MATRIX_DriverHandle_t*`, `LED_MATRIX_Rotation_t` | `void` | Góc xoay áp dụng cho các lần vẽ **sau đó**. |
| `ledMatrixSetBrightness(dev, brightness)` | `const LED_MATRIX_DriverHandle_t*`, `uint8_t` | `void` | Độ sáng 0–15 (lớn hơn bị giới hạn về 15). |
| `ledMatrixDrawPixel(dev, x, y, state)` | `LED_MATRIX_DriverHandle_t*`, `int16_t`, `int16_t`, `bool` | `void` | Bật / tắt 1 điểm; ngoài màn hình bị bỏ qua. |
| `ledMatrixDrawLine(dev, x0, y0, x1, y1, state)` | `LED_MATRIX_DriverHandle_t*`, `int16_t` × 4, `bool` | `void` | Đường thẳng Bresenham. |
| `ledMatrixDrawChar(dev, x, y, ch, state)` | `LED_MATRIX_DriverHandle_t*`, `int16_t`, `int16_t`, `char`, `bool` | `void` | 1 ký tự font 5x7, góc trên trái `(x, y)`; ngoài 32–126 bị bỏ qua. |
| `ledMatrixDrawString(dev, x, y, str, state)` | `LED_MATRIX_DriverHandle_t*`, `int16_t`, `int16_t`, `const char*`, `bool` | `void` | Chuỗi ký tự, mỗi ký tự rộng 6 pixel. |
| `ledMatrixSetScrollDirection(dev, direction)` | `LED_MATRIX_DriverHandle_t*`, `LED_MATRIX_ScrollDirection_t` | `void` | Hướng cuộn; đặt lại vị trí cuộn về mép xuất phát. |
| `ledMatrixScrollTextAnimation(dev, str, y, state)` | `LED_MATRIX_DriverHandle_t*`, `const char*`, `int16_t`, `bool` | `void` | Xóa bộ đệm, vẽ chuỗi tại vị trí cuộn hiện tại rồi dịch 1 pixel (hết đường thì quay vòng). `y` dùng khi cuộn ngang; cuộn dọc chữ đặt ở `x = 0`. Đổi con trỏ chuỗi sẽ cuộn lại từ đầu. Gọi `ledMatrixUpdate()` sau đó. |

### Kiểu dữ liệu

| Kiểu | Thành phần | Mô tả |
| :-- | :-- | :-- |
| `LED_MATRIX_DriverHandle_t` | `display` (`MAX7219_DriverHandle_t`), `frameBuffer`, `rotation`, `scrollDirection`, trạng thái cuộn nội bộ | Một màn hình. Điền bằng `ledMatrixInit()`, không sửa trực tiếp. |
| `LED_MATRIX_Rotation_t` | `LED_MATRIX_ROTATE_0`, `_90`, `_180`, `_270` | Góc xoay. |
| `LED_MATRIX_ScrollDirection_t` | `LED_MATRIX_SCROLL_LEFT`, `_RIGHT`, `_UP`, `_DOWN` | Hướng cuộn. |

Frame Buffer: byte `frameBuffer[khối × 8 + hàng]`, **bit 7 là cột trái** của khối, khối 0 là khối gần chân DIN.

---

## 📄 Giấy phép (License) & Tác giả

- **Tác giả:** [Vọc Vạch IoT](https://github.com/voc-vach-iot)
- **Giấy phép:** Phát hành theo giấy phép [MIT License](LICENSE).
