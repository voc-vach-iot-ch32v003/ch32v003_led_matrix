/**
 * @file ch32v003_led_matrix_defs.h
 * @author Vọc Vạch IoT
 * @brief Hằng số của thư viện LED Matrix 8x8 (kích thước module, font).
 */

#ifndef CH32V003_LED_MATRIX_DEFS_H
#define CH32V003_LED_MATRIX_DEFS_H

// Cho phép người dùng ghi đè cấu hình qua include/sys_config.h của dự án
#if defined(__has_include)
#if __has_include("sys_config.h")
#include "sys_config.h"
#endif
#endif

#define LED_MATRIX_MODULE_SIZE 8 /**< Mỗi module là ma trận 8x8: Frame Buffer cần (số module * 8) byte */
#define LED_MATRIX_CHAR_WIDTH 6 /**< Bề rộng 1 ký tự khi vẽ chuỗi: font 5x7 + 1 cột cách (chuỗi n ký tự rộng n * 6 pixel) */

#endif // CH32V003_LED_MATRIX_DEFS_H
