#include "../lis3dh.hpp"
#include "tusb.h"

#define I2C_PORT i2c0
#define I2C_SDA 12
#define I2C_SCL 13

LIS3DH accel(I2C_PORT);

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    if (accel.begin(16)) {
        printf("Init successful\n");
    } else {
        printf("Init failed\n");
        return 1;
    }
    float x, y, z;

    while (true) {
        accel.read_accel(&x, &y, &z);
        printf("x: %.3f m/s^2\n", x);
        printf("y: %.3f m/s^2\n", y);
        printf("z: %.3f m/s^2\n\n", z);
        sleep_ms(200);
    }

    return 0;
}