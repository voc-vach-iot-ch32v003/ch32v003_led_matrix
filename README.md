# 🧱 ch32v003_led_matrix

Thư viện đồ họa cao cấp điều khiển ma trận đèn LED (**LED Matrix 8x8**) ghép nối tiếp qua IC **MAX7219**, dành riêng cho dòng vi điều khiển **CH32V003** trên nền framework **ch32v003fun**.

Thư viện hỗ trợ quản lý bộ đệm khung hình (Frame Buffer) tối ưu RAM, xoay màn hình linh hoạt, vẽ hình học cơ bản, hiển thị bảng chữ cái ASCII Font 5x7 và tích hợp sẵn hiệu ứng cuộn chữ tự động.

---

## 🚀 Quy trình khởi tạo bắt buộc (Initialization Workflow)

Để hệ thống hiển thị hoạt động chính xác, người dùng **bắt buộc** phải tuân thủ thứ tự 3 bước khởi tạo sau trong hàm `setup()`:

1. **Khởi tạo giao tiếp SPI:** Khởi tạo bus SPI (Hardware SPI hoặc Software SPI) để chuẩn bị đường truyền dữ liệu tới MAX7219.
2. **Khởi tạo LED Matrix:** Gọi hàm `ledMatrixInit()` để liên kết bộ đệm RAM, thiết lập chân Chip Select (CS) và khởi tạo IC MAX7219.
3. **Cấu hình tham số hiển thị:** Thiết lập độ sáng, góc xoay màn hình và hướng cuộn chữ trước khi đi vào vòng lặp chính.

---

## 📑 Cấu trúc hàm (API Reference)

### 1. Hàm khởi tạo & Quản lý hiển thị

| Hàm xử lý                                            | Tham số                                                                         | Giá trị trả về | Mô tả                                                                            |
| ---------------------------------------------------- | ------------------------------------------------------------------------------- | -------------- | -------------------------------------------------------------------------------- |
| **`ledMatrixInit(dev, buffer, matrixCount, csPin)`** | `LedMatrix_t* dev`, `uint8_t* buffer`, `uint8_t matrixCount`, `MCU_Pin_t csPin` | `void`         | Khởi tạo phần cứng MAX7219, gán bộ đệm RAM và thiết lập thông số ban đầu.        |
| **`ledMatrixClearBuffer(dev)`**                      | `LedMatrix_t* dev`                                                              | `void`         | Xóa sạch toàn bộ dữ liệu trong bộ đệm RAM (chưa đẩy ra màn hình).                |
| **`ledMatrixUpdate(dev)`**                           | `LedMatrix_t* dev`                                                              | `void`         | Đẩy toàn bộ dữ liệu từ Frame Buffer ra ma trận LED qua bus SPI.                  |
| **`ledMatrixSetBrightness(dev, brightness)`**        | `LedMatrix_t* dev`, `uint8_t brightness`                                        | `void`         | Thay đổi độ sáng toàn hệ thống ma trận (giá trị từ `0` đến `15`).                |
| **`ledMatrixSetRotation(dev, rotation)`**            | `LedMatrix_t* dev`, `MatrixRotation_t rotation`                                 | `void`         | Cài đặt góc xoay hiển thị (`MATRIX_ROTATE_0`, `MATRIX_ROTATE_90`, `180`, `270`). |
| **`ledMatrixSetScrollDirection(dev, direction)`**    | `LedMatrix_t* dev`, `MatrixScrollDirection_t direction`                         | `void`         | Đặt hướng cuộn chữ (`MATRIX_SCROLL_LEFT`, `RIGHT`, `UP`, `DOWN`).                |

### 2. Hàm vẽ đồ họa & Ký tự

| Hàm xử lý                                              | Tham số                                                                       | Giá trị trả về | Mô tả                                                                         |
| ------------------------------------------------------ | ----------------------------------------------------------------------------- | -------------- | ----------------------------------------------------------------------------- |
| **`ledMatrixDrawPixel(dev, x, y, state)`**             | `LedMatrix_t* dev`, `int16_t x`, `int16_t y`, `bool state`                    | `void`         | Bật (`true`) hoặc Tắt (`false`) 1 điểm ảnh tại tọa độ `(x, y)` theo góc xoay. |
| **`ledMatrixDrawLine(dev, x0, y0, x1, y1, state)`**    | `LedMatrix_t* dev`, `int16_t x0`, `y0`, `x1`, `y1`, `bool state`              | `void`         | Vẽ đoạn thẳng nối giữa 2 điểm bằng thuật toán Bresenham.                      |
| **`ledMatrixDrawChar(dev, x, y, ch, state)`**          | `LedMatrix_t* dev`, `int16_t x`, `int16_t y`, `char ch`, `bool state`         | `void`         | Vẽ 1 ký tự ASCII (Font 5x7) tại góc trên bên trái `(x, y)`.                   |
| **`ledMatrixDrawString(dev, x, y, str, state)`**       | `LedMatrix_t* dev`, `int16_t x`, `int16_t y`, `const char* str`, `bool state` | `void`         | In một chuỗi ký tự cố định lên màn hình tại vị trí chỉ định.                  |
| **`ledMatrixScrollTextAnimation(dev, str, y, state)`** | `LedMatrix_t* dev`, `const char* str`, `int16_t y`, `bool state`              | `void`         | Dịch chuyển chữ chạy 1 bước theo hướng đã cài đặt.                            |

---

## 📝 Code mẫu sử dụng

```c
#include "ch32fun.h"
#include "ch32v003_delay.h"
#include "ch32v003_spi.h"
#include "ch32v003_max7219.h"
#include "ch32v003_led_matrix.h"

#define MATRIX_COUNT    2           // Số lượng module LED Matrix ghép nối tiếp

// Cấu hình chân giao tiếp (Ví dụ trên gói vỏ SOP8 CH32V003_J4M6)
#define SCK_PIN         MCU_PIN1    // Chân Clock SPI
#define DIN_PIN         MCU_PIN3    // Chân MOSI SPI (Data In MAX7219)
#define CS_PIN          MCU_PIN7    // Chân Chip Select (CS)

// Cấp phát bộ đệm RAM (Mỗi module chiếm 8 bytes)
static uint8_t matrixBuffer[MATRIX_COUNT * 8];
static LedMatrix_t myMatrix;

static void setup()
{
    // BƯỚC 1: Khởi tạo Bus SPI (Software SPI hoặc Hardware SPI)
    const SPI_SoftwareConfig_t spiCfg = {
        .sckPin = SCK_PIN,
        .mosiPin = DIN_PIN,
        .misoPin = MCU_PIN_NONE
    };
    spiInitSoftware(spiCfg);

    // BƯỚC 2: Khởi tạo LED Matrix & Tự động cấu hình IC MAX7219
    ledMatrixInit(&myMatrix, matrixBuffer, MATRIX_COUNT, CS_PIN);

    // BƯỚC 3: Cấu hình tham số hiển thị
    ledMatrixSetBrightness(&myMatrix, 3);                   // Độ sáng 3/15
    ledMatrixSetRotation(&myMatrix, MATRIX_ROTATE_270);     // Xoay màn hình 270 độ
    ledMatrixSetScrollDirection(&myMatrix, MATRIX_SCROLL_LEFT); // Hướng cuộn chữ
}

static void loop()
{
    // Cập nhật hoạt ảnh cuộn chữ và đẩy dữ liệu lên ma trận
    ledMatrixScrollTextAnimation(&myMatrix, "Voc Vach IoT", 0, true);
    ledMatrixUpdate(&myMatrix);
    delayMs(70);
}

int main()
{
    SystemInit();
    delayMs(1000); // Tạm dừng chờ mạch nạp / nguồn ổn định

    setup();
    while (1) {
        loop();
    }
}
```
