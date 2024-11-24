#include "../lis3dh.hpp"
#include "tusb.h"

#define I2C_PORT i2c0
#define I2C_SDA 12
#define I2C_SCL 13

LIS3DH accel(I2C_PORT);

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    while (!accel.begin(16)) {
        printf("Error: Accelerometer failed to initialize\n");
    }
    float x, y, z;

    while (true) {
        if (!accel.read_accel(&x, &y, &z)) {
            printf("Error: Accelerometer failed to read acceleration\n");
        }

        printf("Accel X (m/s^2): %.3f\n", x);
        printf("Accel Y (m/s^2): %.3f\n", y);
        printf("Accel Z (m/s^2): %.3f\n\n", z);
        sleep_ms(20);
    }

    return 0;
}