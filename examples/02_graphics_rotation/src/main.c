#include "ch32fun.h"
#include "ch32v003_delay.h"
#include "ch32v003_gpio.h"
#include "ch32v003_led_matrix.h"
#include "ch32v003_spi.h"
#include "user_config.h"

// ============================================================================
// VÍ DỤ: Vẽ điểm, đường thẳng, chữ; lần lượt thử 4 góc xoay để chọn góc đúng với module đang dùng.
// Tọa độ logic: x = 0 .. MATRIX_COUNT * 8 - 1 (trái -> phải), y = 0 .. 7 (trên -> dưới).
// ============================================================================

// SPI phần mềm: chạy trên cả 3 gói vỏ (F4P6 có thể dùng SPI1 phần cứng, xem README thư viện)
static SPI_DriverHandle_t spiBus = {
    .driverMode = SPI_MODE_SOFTWARE,
    .config.software = {.sckPin = MATRIX_CLK_PIN, .mosiPin = MATRIX_DIN_PIN, .misoPin = MCU_PIN_NONE},
};

static LED_MATRIX_DriverHandle_t matrix;
static uint8_t frameBuffer[MATRIX_COUNT * LED_MATRIX_MODULE_SIZE];

static const LED_MATRIX_Rotation_t rotations[] = {LED_MATRIX_ROTATE_0, LED_MATRIX_ROTATE_90, LED_MATRIX_ROTATE_180,
                                                  LED_MATRIX_ROTATE_270};
static const char* const rotationNames[] = {"R0", "R90", "R180", "R270"};

static void setup(void)
{
    spiInit(&spiBus);
    ledMatrixInit(&matrix, &spiBus, MATRIX_CS_PIN, frameBuffer, MATRIX_COUNT);
    ledMatrixSetBrightness(&matrix, 2);
}

static void loop(void)
{
    const int16_t width = MATRIX_COUNT * LED_MATRIX_MODULE_SIZE;

    for (uint8_t i = 0; i < 4; i++)
    {
        ledMatrixSetRotation(&matrix, rotations[i]);

        // 1. Khung viền + 2 đường chéo của khối đầu tiên
        ledMatrixClearBuffer(&matrix);
        ledMatrixDrawLine(&matrix, 0, 0, width - 1, 0, true);
        ledMatrixDrawLine(&matrix, 0, 7, width - 1, 7, true);
        ledMatrixDrawLine(&matrix, 0, 0, 0, 7, true);
        ledMatrixDrawLine(&matrix, width - 1, 0, width - 1, 7, true);
        ledMatrixDrawLine(&matrix, 0, 0, 7, 7, true);
        ledMatrixDrawLine(&matrix, 0, 7, 7, 0, true);
        ledMatrixUpdate(&matrix);
        delayMs(1500);

        // 2. Tên góc xoay: đọc được chữ đúng chiều thì đó là góc hợp với module
        ledMatrixClearBuffer(&matrix);
        ledMatrixDrawString(&matrix, 1, 0, rotationNames[i], true);
        ledMatrixUpdate(&matrix);
        delayMs(2000);
    }
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
