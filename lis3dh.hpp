#include "pico/stdlib.h"
#include "hardware/i2c.h"

#ifndef LIS3DH_
#define LIS3DH_

#define LIS3DH_ADDR (0x19)

#define LIS3DH_REG_CTRL_1 (0x20)
#define LIS3DH_REG_CTRL_4 (0x23)
#define LIS3DH_REG_STATUS (0x27)
#define LIS3DH_REG_OUT_X_L (0x28)
#define LIS3DH_REG_OUT_Y_L (0x2A)
#define LIS3DH_REG_OUT_Z_L (0x2C)

#define LIS3DH_REG_WHO_AM_I (0x0F)
#define LI3DH_WHO_AM_I (0x33)

#define GRAVITY_EARTH (9.80665F)

class LIS3DH {
public:
    LIS3DH(i2c_inst_t *i2c_type);
    bool begin();
    bool read_accel(double *x, double *y, double *z);
    
    uint8_t get_id();
private:
    void write_register(const uint8_t reg, const uint8_t val);
    uint16_t read_register_16(uint8_t reg);

    i2c_inst_t *i2c;
};

#endif