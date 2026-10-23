/**
 * @file ch32v003_led_matrix.h
 * @author Vọc Vạch IoT
 * @brief Thư viện điều khiển LED Matrix 8x8 lớp đồ họa (Frame Buffer, Font 5x7, Line) cho CH32V003.
 */

#ifndef CH32V003_LED_MATRIX_H
#define CH32V003_LED_MATRIX_H

#include "ch32fun.h"
#include "ch32v003_max7219.h"
#include "ch32v003_led_matrix_defs.h"
#include "ch32v003_led_matrix_types.h"

// ============================================================================
// KHAI BÁO HÀM ĐIỀU KHIỂN LED MATRIX
// ============================================================================

/**
 * @brief Khởi tạo hệ thống LED Matrix (chuỗi MAX7219) và xóa màn hình.
 * @param dev Con trỏ Handle LED Matrix.
 * @param spiHandle Bus SPI đã khởi tạo bằng spiInit().
 * @param csPin Chân CS điều khiển (MCU_Pin_t).
 * @param buffer Mảng RAM đệm do người dùng cấp phát (kích thước = matrixCount * 8).
 * @param matrixCount Số lượng module 8x8 ghép nối tiếp.
 * @return uint8_t 1 nếu thành công, 0 nếu tham số không hợp lệ.
 */
uint8_t ledMatrixInit(LED_MATRIX_DriverHandle_t* dev, const SPI_DriverHandle_t* spiHandle, uint8_t csPin,
                      uint8_t* buffer, uint8_t matrixCount);

/**
 * @brief Xóa toàn bộ dữ liệu trong Frame Buffer (không tự động đẩy lên màn hình).
 * @param dev Con trỏ Handle LED Matrix.
 */
void ledMatrixClearBuffer(LED_MATRIX_DriverHandle_t* dev);

/**
 * @brief Đẩy toàn bộ Frame Buffer ra màn hình qua SPI (8 khung CS, mỗi khung cập nhật 1 hàng cho cả chuỗi).
 * @param dev Con trỏ Handle LED Matrix.
 */
void ledMatrixUpdate(const LED_MATRIX_DriverHandle_t* dev);

/**
 * @brief Cài đặt góc xoay hiển thị.
 * @param dev Con trỏ Handle LED Matrix.
 * @param rotation Góc xoay, xem @ref LED_MATRIX_Rotation_t (90/270 xoay từng module, 180 lật cả dải).
 */
void ledMatrixSetRotation(LED_MATRIX_DriverHandle_t* dev, LED_MATRIX_Rotation_t rotation);

/**
 * @brief Cài đặt hướng cuộn chữ cho ledMatrixScrollTextAnimation() (tự reset vị trí cuộn).
 * @param dev Con trỏ Handle LED Matrix.
 * @param direction Hướng cuộn (LEFT, RIGHT, UP, DOWN).
 */
void ledMatrixSetScrollDirection(LED_MATRIX_DriverHandle_t* dev, LED_MATRIX_ScrollDirection_t direction);

/**
 * @brief Cài đặt độ sáng hiển thị (0 .. 15).
 * @param dev Con trỏ Handle LED Matrix.
 * @param brightness Độ sáng (0 .. 15).
 */
void ledMatrixSetBrightness(const LED_MATRIX_DriverHandle_t* dev, uint8_t brightness);

/**
 * @brief Vẽ hoặc xóa 1 điểm ảnh (Pixel) tại tọa độ (x, y). Điểm nằm ngoài màn hình bị bỏ qua.
 * @param dev Con trỏ Handle LED Matrix.
 * @param x Tọa độ X (0 .. matrixCount * 8 - 1).
 * @param y Tọa độ Y (0 .. 7).
 * @param state true: Bật điểm LED, false: Tắt điểm LED.
 */
void ledMatrixDrawPixel(LED_MATRIX_DriverHandle_t* dev, int16_t x, int16_t y, bool state);

/**
 * @brief Vẽ một đoạn thẳng từ (x0, y0) tới (x1, y1) bằng thuật toán Bresenham.
 * @param dev Con trỏ Handle LED Matrix.
 * @param x0 Tọa độ X điểm đầu.
 * @param y0 Tọa độ Y điểm đầu.
 * @param x1 Tọa độ X điểm cuối.
 * @param y1 Tọa độ Y điểm cuối.
 * @param state true: Bật điểm LED, false: Tắt điểm LED.
 */
void ledMatrixDrawLine(LED_MATRIX_DriverHandle_t* dev, int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool state);

/**
 * @brief Vẽ một ký tự ASCII (font 5x7) tại tọa độ góc trên trái (x, y).
 * @param dev Con trỏ Handle LED Matrix.
 * @param x Tọa độ X góc trên trái (cho phép âm để vẽ một phần ký tự).
 * @param y Tọa độ Y góc trên trái.
 * @param ch Ký tự ASCII cần vẽ (32 .. 126).
 * @param state true: Bật điểm LED, false: Tắt điểm LED.
 */
void ledMatrixDrawChar(LED_MATRIX_DriverHandle_t* dev, int16_t x, int16_t y, char ch, bool state);

/**
 * @brief Vẽ một chuỗi ký tự cố định tại tọa độ (x, y), mỗi ký tự rộng 6 pixel.
 * @param dev Con trỏ Handle LED Matrix.
 * @param x Tọa độ X điểm bắt đầu.
 * @param y Tọa độ Y điểm bắt đầu.
 * @param str Chuỗi ký tự cần vẽ.
 * @param state true: Bật điểm LED, false: Tắt điểm LED.
 */
void ledMatrixDrawString(LED_MATRIX_DriverHandle_t* dev, int16_t x, int16_t y, const char* str, bool state);

/**
 * @brief Vẽ 1 bước hiệu ứng cuộn chữ vào Frame Buffer (gọi định kỳ, sau đó gọi ledMatrixUpdate()).
 * @details Trạng thái cuộn lưu trong Handle nên nhiều màn hình có thể cuộn độc lập.
 * @param dev Con trỏ Handle LED Matrix.
 * @param str Chuỗi ký tự cần cuộn (đổi con trỏ chuỗi sẽ cuộn lại từ đầu).
 * @param y Tọa độ Y của chữ khi cuộn ngang (bỏ qua khi cuộn dọc).
 * @param state true: Bật điểm LED, false: Tắt điểm LED.
 */
void ledMatrixScrollTextAnimation(LED_MATRIX_DriverHandle_t* dev, const char* str, int16_t y, bool state);

#endif // CH32V003_LED_MATRIX_H
