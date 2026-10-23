#include "ch32v003_led_matrix.h"
#include <string.h>

// Bảng Font 5x7 chuẩn ASCII (từ mã 32 ' ' đến 126 '~'), mỗi byte là 1 cột, bit 0 = hàng trên cùng
static const uint8_t font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Space (32)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    {0x3E, 0x41, 0x5D, 0x55, 0x1E}, // @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // \.
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // _
    {0x00, 0x01, 0x02, 0x05, 0x00}, // `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // g
    {0x7F, 0x08, 0x04, 0x04, 0x7F}, // h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // l
    {0x7C, 0x04, 0x18, 0x04, 0x7C}, // m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // r
    {0x48, 0x54, 0x54, 0x54, 0x24}, // s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // }
    {0x08, 0x04, 0x08, 0x10, 0x08} // ~
};

uint8_t ledMatrixInit(LED_MATRIX_DriverHandle_t* dev, const SPI_DriverHandle_t* spiHandle, const uint8_t csPin,
                      uint8_t* buffer, const uint8_t matrixCount)
{
    if (dev == NULL || buffer == NULL) return 0;

    dev->display.spiHandle = spiHandle;
    dev->display.csPin = csPin;
    dev->display.cascadeCount = matrixCount;
    dev->frameBuffer = buffer;
    dev->rotation = LED_MATRIX_ROTATE_0;
    dev->scrollDirection = LED_MATRIX_SCROLL_LEFT;
    dev->scrollText = NULL;
    dev->scrollPosition = 0;

    if (!max7219Init(&dev->display)) return 0;

    ledMatrixClearBuffer(dev);
    ledMatrixUpdate(dev);
    return 1;
}

void ledMatrixClearBuffer(LED_MATRIX_DriverHandle_t* dev)
{
    memset(dev->frameBuffer, 0, dev->display.cascadeCount * 8);
}

void ledMatrixUpdate(const LED_MATRIX_DriverHandle_t* dev)
{
    // Mỗi hàng chỉ tốn 1 khung CS cho cả chuỗi: dữ liệu module i nằm tại frameBuffer[i * 8 + row]
    for (uint8_t row = 0; row < 8; row++)
    {
        max7219WriteChain(&dev->display, MAX7219_REG_DIGIT0 + row, &dev->frameBuffer[row], 8);
    }
}

void ledMatrixSetRotation(LED_MATRIX_DriverHandle_t* dev, const LED_MATRIX_Rotation_t rotation)
{
    dev->rotation = rotation;
}

void ledMatrixSetScrollDirection(LED_MATRIX_DriverHandle_t* dev, const LED_MATRIX_ScrollDirection_t direction)
{
    dev->scrollDirection = direction;
    dev->scrollText = NULL; // Buộc tính lại vị trí xuất phát theo hướng mới
}

void ledMatrixSetBrightness(const LED_MATRIX_DriverHandle_t* dev, const uint8_t brightness)
{
    max7219SetIntensity(&dev->display, brightness);
}

void ledMatrixDrawPixel(LED_MATRIX_DriverHandle_t* dev, const int16_t x, const int16_t y, const bool state)
{
    const int16_t totalWidth = (int16_t)dev->display.cascadeCount * 8;
    if (x < 0 || x >= totalWidth || y < 0 || y >= 8) return;

    // Tách tọa độ thành (module, cột cục bộ) rồi xoay trong phạm vi 8x8 -> không bao giờ ghi tràn bộ đệm
    uint8_t module = (uint8_t)(x >> 3);
    uint8_t col = (uint8_t)(x & 7);
    uint8_t row = (uint8_t)y;

    switch (dev->rotation)
    {
    case LED_MATRIX_ROTATE_90:
        {
            const uint8_t oldCol = col;
            col = row;
            row = 7 - oldCol;
            break;
        }
    case LED_MATRIX_ROTATE_180:
        module = dev->display.cascadeCount - 1 - module;
        col = 7 - col;
        row = 7 - row;
        break;
    case LED_MATRIX_ROTATE_270:
        {
            const uint8_t oldCol = col;
            col = 7 - row;
            row = oldCol;
            break;
        }
    default:
        break;
    }

    const uint8_t mask = 1 << (7 - col);
    uint8_t* const cell = &dev->frameBuffer[module * 8 + row];
    if (state)
    {
        *cell |= mask;
    }
    else
    {
        *cell &= ~mask;
    }
}

void ledMatrixDrawLine(LED_MATRIX_DriverHandle_t* dev, int16_t x0, int16_t y0, const int16_t x1, const int16_t y1,
                       const bool state)
{
    const int16_t dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    const int16_t sx = (x0 < x1) ? 1 : -1;
    const int16_t dy = (y1 > y0) ? (y0 - y1) : (y1 - y0);
    const int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx + dy;

    while (1)
    {
        ledMatrixDrawPixel(dev, x0, y0, state);
        if (x0 == x1 && y0 == y1) break;
        const int16_t e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void ledMatrixDrawChar(LED_MATRIX_DriverHandle_t* dev, const int16_t x, const int16_t y, const char ch,
                       const bool state)
{
    if (ch < 32 || ch > 126) return; // Chỉ xử lý ASCII hiển thị được
    const uint8_t* glyph = font5x7[ch - 32];

    for (uint8_t col = 0; col < 5; col++)
    {
        const uint8_t colBits = glyph[col];
        for (uint8_t row = 0; row < 7; row++)
        {
            if (colBits & (1 << row))
            {
                ledMatrixDrawPixel(dev, x + col, y + row, state);
            }
        }
    }
}

void ledMatrixDrawString(LED_MATRIX_DriverHandle_t* dev, int16_t x, const int16_t y, const char* str,
                         const bool state)
{
    const int16_t totalWidth = (int16_t)dev->display.cascadeCount * 8;
    while (*str && x < totalWidth) // Dừng sớm khi ký tự đã nằm ngoài mép phải
    {
        ledMatrixDrawChar(dev, x, y, *str, state);
        x += LED_MATRIX_CHAR_WIDTH;
        str++;
    }
}

void ledMatrixScrollTextAnimation(LED_MATRIX_DriverHandle_t* dev, const char* str, const int16_t y, const bool state)
{
    const int16_t totalWidth = (int16_t)dev->display.cascadeCount * 8;
    const int16_t textWidth = (int16_t)(strlen(str) * LED_MATRIX_CHAR_WIDTH);
    const bool horizontal = (dev->scrollDirection == LED_MATRIX_SCROLL_LEFT ||
        dev->scrollDirection == LED_MATRIX_SCROLL_RIGHT);

    // Chuỗi thay đổi (hoặc vừa đổi hướng) -> đặt lại vị trí xuất phát ngoài mép màn hình
    if (str != dev->scrollText)
    {
        dev->scrollText = str;
        switch (dev->scrollDirection)
        {
        case LED_MATRIX_SCROLL_LEFT: dev->scrollPosition = totalWidth;
            break;
        case LED_MATRIX_SCROLL_RIGHT: dev->scrollPosition = -textWidth;
            break;
        case LED_MATRIX_SCROLL_UP: dev->scrollPosition = 8;
            break;
        default: dev->scrollPosition = -8;
            break;
        }
    }

    // Xóa bộ đệm và vẽ chuỗi tại vị trí hiện tại (dọc: chữ đứng yên ở x = 0, chạy theo trục y)
    ledMatrixClearBuffer(dev);
    if (horizontal)
    {
        ledMatrixDrawString(dev, dev->scrollPosition, y, str, state);
    }
    else
    {
        ledMatrixDrawString(dev, 0, dev->scrollPosition, str, state);
    }

    // Dịch 1 pixel theo hướng cấu hình, chạy hết thì quay vòng
    switch (dev->scrollDirection)
    {
    case LED_MATRIX_SCROLL_LEFT:
        if (--dev->scrollPosition < -textWidth) dev->scrollPosition = totalWidth;
        break;
    case LED_MATRIX_SCROLL_RIGHT:
        if (++dev->scrollPosition > totalWidth) dev->scrollPosition = -textWidth;
        break;
    case LED_MATRIX_SCROLL_UP:
        if (--dev->scrollPosition < -8) dev->scrollPosition = 8;
        break;
    default:
        if (++dev->scrollPosition > 8) dev->scrollPosition = -8;
        break;
    }
}
