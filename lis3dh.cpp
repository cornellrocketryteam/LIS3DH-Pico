#include "lis3dh.hpp"
#include <cstdio>

LIS3DH::LIS3DH(i2c_inst_t *i2c_type) {
    i2c = i2c_type;
}

bool LIS3DH::begin() {
    uint8_t whoami = get_id();
    if (whoami != LI3DH_WHO_AM_I) {
        fprintf(stderr, "Error: Got whoami value of %d\n", whoami);
        return false;
    }

    write_register(LIS3DH_REG_CTRL_1, 0x97);

    return true;
}

bool LIS3DH::read_accel(float *x, float *y, float *z) {

    int16_t x_temp = (int16_t)read_register_16(LIS3DH_REG_OUT_X_L);
    int16_t y_temp = (int16_t)read_register_16(LIS3DH_REG_OUT_Y_L);
    int16_t z_temp = (int16_t)read_register_16(LIS3DH_REG_OUT_Z_L);

    // TODO: Consider auto-incrementing
    *x = (4 * (float)x_temp / 64000) * GRAVITY_EARTH;
    *y = (4 * (float)y_temp / 64000) * GRAVITY_EARTH;
    *z = (4 * (float)z_temp / 64000) * GRAVITY_EARTH;

    return true;
    
}

uint16_t LIS3DH::read_register_16(uint8_t reg) {
    uint8_t lsb;
    uint8_t msb;
    uint16_t raw_accel;

    i2c_write_blocking(i2c, LIS3DH_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c, LIS3DH_ADDR, &lsb, 1, false);

    reg |= 0x01;
    i2c_write_blocking(i2c, LIS3DH_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c, LIS3DH_ADDR, &msb, 1, false);

    raw_accel = (msb << 8) | lsb;

    return raw_accel;
}


uint8_t LIS3DH::get_id() {
    uint8_t val[1];
    uint8_t reg[1] = { LIS3DH_REG_WHO_AM_I };

    i2c_write_blocking(i2c, LIS3DH_ADDR, reg, 1, true);
    i2c_read_blocking(i2c, LIS3DH_ADDR, val, 1, false);

    return val[0];
}


void LIS3DH::write_register(const uint8_t reg, const uint8_t val) {
    uint8_t buf[2];

    buf[0] = reg;
    buf[1] = val;
    i2c_write_blocking(i2c, LIS3DH_ADDR, buf, 2, false);
}