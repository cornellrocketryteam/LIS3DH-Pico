#include "lis3dh.hpp"
#include <cstdio>

LIS3DH::LIS3DH(i2c_inst_t *i2c_type) {
    i2c = i2c_type;
}

bool LIS3DH::begin(int g_range) {
    uint8_t whoami = get_id();
    if (whoami != LI3DH_WHO_AM_I) {
#ifdef VERBOSE
        fprintf(stderr, "Error: Accelerometer got whoami value of %d\n", whoami);
#endif
        return false;
    }

    if (write_register(LIS3DH_REG_CTRL_1, 0x97) < 1) {
        return false;
    }

    uint8_t config;

    switch (g_range) {
    case 2:
        config = 0b00000000;
        lsb = 4;
        break;
    case 4:
        config = 0b00010000;
        lsb = 8;
        break;
    case 8:
        config = 0b00100000;
        lsb = 16;
        break;
    case 16:
        config = 0b00110000;
        lsb = 48;
        break;
    default:
#ifdef VERBOSE
        fprintf(stderr, "Error: Invalid G range for accelerometer\n");
#endif
        return false;
    }

    if (write_register(LIS3DH_REG_CTRL_4, config) < 1) {
        return false;
    }

    return true;
}

bool LIS3DH::read_accel(float *x, float *y, float *z) {
    int16_t x_raw, y_raw, z_raw;

    uint8_t raw[6] = {0};

    // MSB of 1 indicates register auto-increment
    uint8_t reg = LIS3DH_REG_OUT_X_L | 0x80;

    if (i2c_write_timeout_us(i2c, LIS3DH_ADDR, &reg, 1, true, LIS3DH_BYTE_TIMEOUT_US) < 1) {
        return false;
    }
    if (i2c_read_timeout_us(i2c, LIS3DH_ADDR, raw, 6, false, 6 * LIS3DH_BYTE_TIMEOUT_US) < 1) {
        return false;
    }

    x_raw = (int16_t)(raw[0] | raw[1] << 8);
    y_raw = (int16_t)(raw[2] | raw[3] << 8);
    z_raw = (int16_t)(raw[4] | raw[5] << 8);

    *x = (lsb * (float)x_raw / 64000);
    *y = (lsb * (float)y_raw / 64000);
    *z = (lsb * (float)z_raw / 64000);

    return true;
}

uint8_t LIS3DH::get_id() {
    uint8_t val;
    uint8_t reg = LIS3DH_REG_WHO_AM_I;

    i2c_write_timeout_us(i2c, LIS3DH_ADDR, &reg, 1, true, LIS3DH_BYTE_TIMEOUT_US);
    i2c_read_timeout_us(i2c, LIS3DH_ADDR, &val, 1, false, LIS3DH_BYTE_TIMEOUT_US);

    return val;
}

bool LIS3DH::write_register(const uint8_t reg, const uint8_t val) {
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = val;

    if (i2c_write_timeout_us(i2c, LIS3DH_ADDR, buf, 2, false, 2 * LIS3DH_BYTE_TIMEOUT_US) < 1) {
        return false;
    }

    return true;
}