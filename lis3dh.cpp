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

    uint8_t config[1];
    switch (g_range) {
    case 2:
        config[0] = 0b00000000;
        this->lsb = 4;
        break;
    case 4:
        config[0] = 0b00010000;
        this->lsb = 8;
        break;
    case 8:
        config[0] = 0b00100000;
        this->lsb = 16;
        break;
    case 16:
        config[0] = 0b00110000;
        this->lsb = 48;
        break;
    default:
#ifdef VERBOSE
        fprintf(stderr, "Error: Invalid G range\n");
#endif
        return false;
    }

    if (write_register(LIS3DH_REG_CTRL_4, config[0]) < 1) {
        return false;
    }

    return true;
}

bool LIS3DH::read_accel(float *x, float *y, float *z) {

    int16_t x_raw, y_raw, z_raw;
    ret = read_raw_accel(LIS3DH_REG_OUT_X_L, &x_raw);
    if (!ret) {
        return false;
    }

    ret = read_raw_accel(LIS3DH_REG_OUT_Y_L, &y_raw);
    if (!ret) {
        return false;
    }

    ret = read_raw_accel(LIS3DH_REG_OUT_Z_L, &z_raw);
    if (!ret) {
        return false;
    }

    // TODO: Consider auto-incrementing
    *x = (this->lsb * (float)x_raw / 64000);
    *y = (this->lsb * (float)y_raw / 64000);
    *z = (this->lsb * (float)z_raw / 64000);

    return true;
}

bool LIS3DH::read_raw_accel(uint8_t reg, int16_t *val) {
    uint8_t lsb, msb;

    ret = i2c_write_blocking(i2c, LIS3DH_ADDR, &reg, 1, true);
    if (ret < 1) {
        return false;
    }

    ret = i2c_read_blocking(i2c, LIS3DH_ADDR, &lsb, 1, false);
    if (ret < 1) {
        return false;
    }

    reg |= 0x01;
    ret = i2c_write_blocking(i2c, LIS3DH_ADDR, &reg, 1, true);
    if (ret < 1) {
        return false;
    }

    ret = i2c_read_blocking(i2c, LIS3DH_ADDR, &msb, 1, false);
    if (ret < 1) {
        return false;
    }

    *val = (msb << 8) | lsb;

    return true;
}

uint8_t LIS3DH::get_id() {
    uint8_t val[1];
    uint8_t reg[1] = {LIS3DH_REG_WHO_AM_I};

    i2c_write_blocking(i2c, LIS3DH_ADDR, reg, 1, true);
    i2c_read_blocking(i2c, LIS3DH_ADDR, val, 1, false);

    return val[0];
}

bool LIS3DH::write_register(const uint8_t reg, const uint8_t val) {
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = val;

    ret = i2c_write_blocking(i2c, LIS3DH_ADDR, buf, 2, false);
    if (ret < 1) {
        return false;
    }

    return true;
}