/**
 * @file ch32v003_led_matrix_types.h
 * @author Vọc Vạch IoT
 * @brief Định nghĩa kiểu dữ liệu và Struct Handle cho LED Matrix 8x8 (chuỗi MAX7219).
 */

#ifndef CH32V003_LED_MATRIX_TYPES_H
#define CH32V003_LED_MATRIX_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include "ch32v003_max7219.h"

/**
 * @brief Chế độ xoay hiển thị.
 * @details 90/270 độ xoay TỪNG module 8x8 tại chỗ (dùng khi module bị hàn xoay), 180 độ lật toàn
 *          bộ dải module (dùng khi cả dải bị lắp ngược). Tọa độ logic luôn là (cascadeCount * 8) x 8.
 */
typedef enum
{
    LED_MATRIX_ROTATE_0 = 0, /**< Không xoay */
    LED_MATRIX_ROTATE_90, /**< Xoay từng module 90 độ */
    LED_MATRIX_ROTATE_180, /**< Lật toàn bộ dải 180 độ */
    LED_MATRIX_ROTATE_270 /**< Xoay từng module 270 độ */
} LED_MATRIX_Rotation_t;

/**
 * @brief Hướng cuộn chữ/hiệu ứng trên màn hình Matrix.
 */
typedef enum
{
    LED_MATRIX_SCROLL_LEFT = 0, /**< Cuộn nội dung sang bên trái (Chữ chạy từ Phải qua Trái) */
    LED_MATRIX_SCROLL_RIGHT, /**< Cuộn nội dung sang bên phải (Chữ chạy từ Trái qua Phải) */
    LED_MATRIX_SCROLL_UP, /**< Cuộn nội dung lên phía trên */
    LED_MATRIX_SCROLL_DOWN /**< Cuộn nội dung xuống phía dưới */
} LED_MATRIX_ScrollDirection_t;

/**
 * @brief Struct Handle quản lý đối tượng LED Matrix.
 */
typedef struct
{
    MAX7219_DriverHandle_t display; /**< Chuỗi MAX7219 điều khiển các module (cascadeCount = số module) */
    uint8_t* frameBuffer; /**< Bộ đệm khung hình do người dùng cấp (cascadeCount * 8 byte) */
    LED_MATRIX_Rotation_t rotation; /**< Góc xoay hiển thị */
    LED_MATRIX_ScrollDirection_t scrollDirection; /**< Hướng cuộn cho ledMatrixScrollTextAnimation() */
    const char* scrollText; /**< [NỘI BỘ] Chuỗi đang cuộn (đổi chuỗi -> tự reset vị trí) */
    int16_t scrollPosition; /**< [NỘI BỘ] Vị trí cuộn hiện tại (pixel) */
} LED_MATRIX_DriverHandle_t;

#endif // CH32V003_LED_MATRIX_TYPES_H
