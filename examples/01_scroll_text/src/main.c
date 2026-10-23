#include "ch32fun.h"
#include "ch32v003_delay.h"
#include "ch32v003_gpio.h"
#include "ch32v003_led_matrix.h"
#include "ch32v003_spi.h"
#include "user_config.h"

// ============================================================================
// VÍ DỤ: Cuộn chữ trên ma trận LED 4 khối: chạy sang trái 2 lượt, rồi cuộn lên / xuống.
// Mỗi lần gọi ledMatrixScrollTextAnimation() dịch chữ 1 pixel vào Frame Buffer, ledMatrixUpdate() đẩy ra màn hình.
// ============================================================================

// SPI phần mềm: chạy trên cả 3 gói vỏ (F4P6 có thể dùng SPI1 phần cứng, xem README thư viện)
static SPI_DriverHandle_t spiBus = {
    .driverMode = SPI_MODE_SOFTWARE,
    .config.software = {.sckPin = MATRIX_CLK_PIN, .mosiPin = MATRIX_DIN_PIN, .misoPin = MCU_PIN_NONE},
};

static LED_MATRIX_DriverHandle_t matrix;
static uint8_t frameBuffer[MATRIX_COUNT * LED_MATRIX_MODULE_SIZE];

static const char message[] = "Hello CH32V003!";
static const char shortText[] = "IoT";

// Chạy đủ 1 vòng: chữ đi từ mép này ra hết mép kia
static void scrollOnce(const LED_MATRIX_ScrollDirection_t direction, const char* text, const uint16_t steps,
                       const uint16_t stepMs)
{
    ledMatrixSetScrollDirection(&matrix, direction);
    for (uint16_t i = 0; i < steps; i++)
    {
        ledMatrixScrollTextAnimation(&matrix, text, 0, true);
        ledMatrixUpdate(&matrix);
        delayMs(stepMs);
    }
}

static void setup(void)
{
    spiInit(&spiBus);
    ledMatrixInit(&matrix, &spiBus, MATRIX_CS_PIN, frameBuffer, MATRIX_COUNT);
    ledMatrixSetBrightness(&matrix, 2);
}

static void loop(void)
{
    // Ngang: quãng đường = bề rộng màn hình + bề rộng chuỗi
    const uint16_t horizontal = MATRIX_COUNT * LED_MATRIX_MODULE_SIZE + (sizeof(message) - 1) * LED_MATRIX_CHAR_WIDTH;
    scrollOnce(LED_MATRIX_SCROLL_LEFT, message, horizontal, 30);
    scrollOnce(LED_MATRIX_SCROLL_LEFT, message, horizontal, 30);

    // Dọc: chữ đi từ mép dưới lên hết mép trên (8 + 8 bước), rồi ngược lại
    scrollOnce(LED_MATRIX_SCROLL_UP, shortText, 17, 60);
    scrollOnce(LED_MATRIX_SCROLL_DOWN, shortText, 17, 60);
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
