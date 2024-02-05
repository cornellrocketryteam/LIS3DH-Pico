#include "lis3dh.hpp"
#include <cstdio>

LIS3DH::LIS3DH(i2c_inst_t *i2c_type) {
    i2c = i2c_type;
}

bool LIS3DH::begin() {
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

    return true;
}

bool LIS3DH::read_accel(float *x, float *y, float *z) {

    int16_t x_raw, y_raw, z_raw;
    ret = read_raw_accel(LIS3DH_REG_OUT_X_L, &x_raw);
    ret = read_raw_accel(LIS3DH_REG_OUT_Y_L, &y_raw);
    ret = read_raw_accel(LIS3DH_REG_OUT_Z_L, &z_raw);

    // TODO: Consider auto-incrementing
    *x = (4 * (float)x_raw / 64000) * GRAVITY_EARTH;
    *y = (4 * (float)y_raw / 64000) * GRAVITY_EARTH;
    *z = (4 * (float)z_raw / 64000) * GRAVITY_EARTH;

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