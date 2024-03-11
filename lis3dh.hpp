#ifndef LIS3DH_HPP
#define LIS3DH_HPP

#include "hardware/i2c.h"
#include "pico/stdlib.h"

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

/**
 * Representation of the LIS3DH sensor.
 */
class LIS3DH {
public:
    /**
     * Initializes an LIS3DH object on an I2C bus.
     * @param i2c_type The I2C bus that this sensor is on
     */
    LIS3DH(i2c_inst_t *i2c_type);

    /**
     * Attempts to establish a connection with the sensor and initiates a software reset.
     * @param g_range The full-scale range of the sensor
     * @return True on successful connection, false otherwise
     */
    bool begin(int g_range = 16);

    /**
     * Reads 3-axis acceleration values in G's.
     * @param x The resulting x-axis acceleration
     * @param y The resulting y-axis acceleration
     * @param z The resulting z-axis acceleration
     * @return True on successful read, false otherwise
     */
    bool read_accel(float *x, float *y, float *z);

private:
    /**
     * Reads the LIS3DH's electronic ID.
     * @return The resulting electronic ID
     */
    uint8_t get_id();

    /**
     * Writes to a register.
     * @param reg The address of the register to write to
     * @param val The byte of data to write
     * @return True on successful write, false otherwise
     */
    bool write_register(const uint8_t reg, const uint8_t val);

    /**
     * Reads a single raw acceleration value.
     * @param reg The register to read from
     * @param val The resulting 16 bit acceleration value
     * @return True on successful read, false otherwise
     */
    bool read_raw_accel(uint8_t reg, int16_t *val);

    /**
     * The scaling factor for acceleration values.
     * This is dependent on the full-scale range.
     */
    int lsb;

    /**
     * Return value for I2C reads and writes.
     */
    int ret;

    /**
     * The I2C bus.
     */
    i2c_inst_t *i2c;
};

#endif // LIS3DH_HPP