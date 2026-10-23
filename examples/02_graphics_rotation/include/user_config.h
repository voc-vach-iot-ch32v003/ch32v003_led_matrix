/**
 * @file user_config.h
 * @author Vọc Vạch IoT
 * @brief Chân nối module ma trận LED 4 khối (FC-16) của ví dụ vẽ đồ họa.
 */

#ifndef USER_CONFIG_H
#define USER_CONFIG_H

/** @brief Chân CS / LOAD của module (chân vật lý số 3: J4M6 PA2, A4M6 PC3, F4P6 PD6). */
#define MATRIX_CS_PIN MCU_PIN3

/** @brief Chân CLK của module (chân vật lý số 5: J4M6 PC1, A4M6 PC6, F4P6 PA1). */
#define MATRIX_CLK_PIN MCU_PIN5

/** @brief Chân DIN của module (chân vật lý số 6: J4M6 PC2, A4M6 PC7, F4P6 PA2). */
#define MATRIX_DIN_PIN MCU_PIN6

/** @brief Số khối 8x8 nối tiếp (module FC-16 có 4 khối). */
#define MATRIX_COUNT 4

#endif // USER_CONFIG_H
