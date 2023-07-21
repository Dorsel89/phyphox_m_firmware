//#define DT_DRV_COMPAT triaxis_mlx90393

#include "mlx90393.h"

#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>

enum mlx90393_resolution _res_x, _res_y, _res_z;
enum mlx90393_gain _gain;
enum mlx90393_filter _dig_filt;
enum mlx90393_oversampling _osr;

uint8_t transceive(uint8_t *txbuf, uint8_t txlen,
                    uint8_t *rxbuf, uint8_t rxlen,
                    uint8_t interdelay, struct device *mlx_device_t) {
  uint8_t status = 0;
  uint8_t i;
  uint8_t rxbuf2[rxlen + 2];

  if (mlx_device_t == NULL) {
    return false;
  }
  
  if (!device_is_ready(mlx_device_t)) {
    return false;
  }

  //Write stage
  
  if (i2c_write(mlx_device_t,txbuf, txlen, MLX90393_DEFAULT_ADDR)) {
    return MLX90393_STATUS_ERROR;
  }
  k_sleep(K_MSEC(interdelay));

  //Read status byte plus any others 
  if (i2c_read(mlx_device_t,rxbuf2, rxlen + 1,MLX90393_DEFAULT_ADDR)) {
    return MLX90393_STATUS_ERROR;
  }
  status = rxbuf2[0];
  for (i = 0; i < rxlen; i++) {
    rxbuf[i] = rxbuf2[i + 1];
  }

  //Mask out bytes available in the status response. 
  return (status >> 2);
}

bool writeRegister(uint8_t reg, uint16_t data, struct device *mlx_device_t) {
  uint8_t tx[4] = {MLX90393_REG_WR,
                   data >> 8,   // high byte
                   data & 0xFF, // low byte
                   reg << 2};   // the register itself, shift up by 2 bits!

  /* Perform the transaction. */
  return (transceive(tx, sizeof(tx), NULL, 0, 0, mlx_device_t) == MLX90393_STATUS_OK);
}

bool readRegister(uint8_t reg, uint16_t *data, struct device *mlx_device_t) {
  uint8_t tx[2] = {MLX90393_REG_RR,reg << 2}; // the register itself, shift up by 2 bits!

  uint8_t rx[2];

  /* Perform the transaction. */
  if (transceive(tx, sizeof(tx), rx, sizeof(rx), 0, mlx_device_t) != MLX90393_STATUS_OK) {
    return false;
  }

  *data = ((uint16_t)rx[0] << 8) | rx[1];

  return true;
}

/**
 * Perform a mode exit
 * @return True if the operation succeeded, otherwise false.
 */
bool mlx_exitMode(struct device *mlx_device_t) {
  uint8_t tx[1] = {MLX90393_REG_EX};

  /* Perform the transaction. */
  return (transceive(tx, sizeof(tx), NULL, 0, 0, mlx_device_t) == MLX90393_STATUS_OK);
}

/**
 * Perform a soft reset
 * @return True if the operation succeeded, otherwise false.
 */
bool mlx_reset(struct device *mlx_device_t) {
  uint8_t tx[1] = {MLX90393_REG_RT};

  /* Perform the transaction. */
  if (transceive(tx, sizeof(tx), NULL, 0, 5, mlx_device_t) != MLX90393_STATUS_RESET) {
    return false;
  }
  return true;
}

/**
 * Sets the sensor gain to the specified level.
 * @param gain  The gain level to set.
 * @return True if the operation succeeded, otherwise false.
 */
bool mlx_setGain(mlx90393_gain_t gain, struct device *mlx_device_t) {
  _gain = gain;

  uint16_t data;
  readRegister(MLX90393_CONF1, &data, mlx_device_t);

  // mask off gain bits
  data &= ~0x0070;
  // set gain bits
  data |= gain << MLX90393_GAIN_SHIFT;

  return writeRegister(MLX90393_CONF1, data, mlx_device_t);
}

/**
 * Gets the current sensor gain.
 *
 * @return An enum containing the current gain level.
 */
mlx90393_gain_t mlx_getGain(struct device *mlx_device_t) {
  uint16_t data;
  readRegister(MLX90393_CONF1, &data, mlx_device_t);

  // mask off gain bits
  data &= 0x0070;

  return (mlx90393_gain_t)(data >> 4);
}

/**
 * Sets the sensor resolution to the specified level.
 * @param axis  The axis to set.
 * @param resolution  The resolution level to set.
 * @return True if the operation succeeded, otherwise false.
 */
bool mlx_setResolution(enum mlx90393_axis axis,
                                      enum mlx90393_resolution resolution, struct device *mlx_device_t) {

  uint16_t data;
  readRegister(MLX90393_CONF3, &data, mlx_device_t);

  switch (axis) {
  case MLX90393_X:
    _res_x = resolution;
    data &= ~0x0060;
    data |= resolution << 5;
    break;
  case MLX90393_Y:
    _res_y = resolution;
    data &= ~0x0180;
    data |= resolution << 7;
    break;
  case MLX90393_Z:
    _res_z = resolution;
    data &= ~0x0600;
    data |= resolution << 9;
    break;
  }

  return writeRegister(MLX90393_CONF3, data, mlx_device_t);
}

/**
 * Gets the current sensor resolution.
 * @param axis  The axis to get.
 * @return An enum containing the current resolution.
 */
enum mlx90393_resolution mlx_getResolution(enum mlx90393_axis axis) {
  switch (axis) {
  case MLX90393_X:
    return _res_x;
  case MLX90393_Y:
    return _res_y;
  case MLX90393_Z:
    return _res_z;
  }

  return 4;
}

/**
 * Sets the digital filter.
 * @param filter The digital filter setting.
 * @return True if the operation succeeded, otherwise false.
 */
bool mlx_setFilter(enum mlx90393_filter filter, struct device *mlx_device_t) {
  _dig_filt = filter;

  uint16_t data;
  readRegister(MLX90393_CONF3, &data, mlx_device_t);

  data &= ~0x1C;
  data |= filter << 2;

  return writeRegister(MLX90393_CONF3, data, mlx_device_t);
}

/**
 * Gets the current digital filter setting.
 * @return An enum containing the current digital filter setting.
 */
enum mlx90393_filter mlx_getFilter(void) { return _dig_filt; }

/**
 * Sets the oversampling.
 * @param oversampling The oversampling value to use.
 * @return True if the operation succeeded, otherwise false.
 */
bool mlx_setOversampling(
    enum mlx90393_oversampling oversampling, struct device *mlx_device_t) {
  _osr = oversampling;

  uint16_t data;
  readRegister(MLX90393_CONF3, &data, mlx_device_t);

  data &= ~0x03;
  data |= oversampling;

  return writeRegister(MLX90393_CONF3, data, mlx_device_t);
}

/**
 * Gets the current oversampling setting.
 * @return An enum containing the current oversampling setting.
 */
enum mlx90393_oversampling mlx_getOversampling(void) {
  return _osr;
}

/**
 * Sets the TRIG_INT pin to the specified function.
 *
 * @param state  'true/1' sets the pin to INT, 'false/0' to TRIG.
 *
 * @return True if the operation succeeded, otherwise false.
 */
bool mlx_setTrigInt(bool state, struct device *mlx_device_t) {
  uint16_t data;
  readRegister(MLX90393_CONF2, &data, mlx_device_t);

  // mask off trigint bit
  data &= ~0x8000;

  // set trigint bit if desired
  if (state) {
    /* Set the INT, highest bit */
    data |= 0x8000;
  }

  return writeRegister(MLX90393_CONF2, data, mlx_device_t);
}

/**
 * Begin a single measurement on all axes
 *
 * @return True on command success
 */
uint8_t startSingleMeasurement(struct device *mlx_device_t) {
  uint8_t tx[1] = {MLX90393_REG_SM | MLX90393_AXIS_ALL};

  /* Set the device to single measurement mode */
  uint8_t stat = transceive(tx, sizeof(tx), NULL, 0, 0, mlx_device_t);
  
  if ((stat == MLX90393_STATUS_OK) || (stat == MLX90393_STATUS_SMMODE)) {
    return stat;
  }
  return stat;
}

uint8_t startBurstMode(struct device *mlx_device_t){
    uint8_t tx[1] = {MLX90393_REG_SB | MLX90393_AXIS_ALL};  
    uint8_t stat = transceive(tx, sizeof(tx), NULL, 0, 0,mlx_device_t);
    if ((stat == MLX90393_STATUS_OK) || (stat == MLX90393_STATUS_SMMODE)) {
        return true;
    }
    return false;  
}

/** Lookup table to convert raw values to uT based on [HALLCONF][GAIN_SEL][RES].
 */
const float mlx90393_lsb_lookup[2][8][4][2] = {

    /* HALLCONF = 0xC (default) */
    {
        /* GAIN_SEL = 0, 5x gain */
        {{0.751, 1.210}, {1.502, 2.420}, {3.004, 4.840}, {6.009, 9.680}},
        /* GAIN_SEL = 1, 4x gain */
        {{0.601, 0.968}, {1.202, 1.936}, {2.403, 3.872}, {4.840, 7.744}},
        /* GAIN_SEL = 2, 3x gain */
        {{0.451, 0.726}, {0.901, 1.452}, {1.803, 2.904}, {3.605, 5.808}},
        /* GAIN_SEL = 3, 2.5x gain */
        {{0.376, 0.605}, {0.751, 1.210}, {1.502, 2.420}, {3.004, 4.840}},
        /* GAIN_SEL = 4, 2x gain */
        {{0.300, 0.484}, {0.601, 0.968}, {1.202, 1.936}, {2.403, 3.872}},
        /* GAIN_SEL = 5, 1.667x gain */
        {{0.250, 0.403}, {0.501, 0.807}, {1.001, 1.613}, {2.003, 3.227}},
        /* GAIN_SEL = 6, 1.333x gain */
        {{0.200, 0.323}, {0.401, 0.645}, {0.801, 1.291}, {1.602, 2.581}},
        /* GAIN_SEL = 7, 1x gain */
        {{0.150, 0.242}, {0.300, 0.484}, {0.601, 0.968}, {1.202, 1.936}},
    },

    /* HALLCONF = 0x0 */
    {
        /* GAIN_SEL = 0, 5x gain */
        {{0.787, 1.267}, {1.573, 2.534}, {3.146, 5.068}, {6.292, 10.137}},
        /* GAIN_SEL = 1, 4x gain */
        {{0.629, 1.014}, {1.258, 2.027}, {2.517, 4.055}, {5.034, 8.109}},
        /* GAIN_SEL = 2, 3x gain */
        {{0.472, 0.760}, {0.944, 1.521}, {1.888, 3.041}, {3.775, 6.082}},
        /* GAIN_SEL = 3, 2.5x gain */
        {{0.393, 0.634}, {0.787, 1.267}, {1.573, 2.534}, {3.146, 5.068}},
        /* GAIN_SEL = 4, 2x gain */
        {{0.315, 0.507}, {0.629, 1.014}, {1.258, 2.027}, {2.517, 4.055}},
        /* GAIN_SEL = 5, 1.667x gain */
        {{0.262, 0.422}, {0.524, 0.845}, {1.049, 1.689}, {2.097, 3.379}},
        /* GAIN_SEL = 6, 1.333x gain */
        {{0.210, 0.338}, {0.419, 0.676}, {0.839, 1.352}, {1.678, 2.703}},
        /* GAIN_SEL = 7, 1x gain */
        {{0.157, 0.253}, {0.315, 0.507}, {0.629, 1.014}, {1.258, 2.027}},
    }
};

/**
 * Reads data from data register & returns the results.
 *
 * @param x     Pointer to where the 'x' value should be stored.
 * @param y     Pointer to where the 'y' value should be stored.
 * @param z     Pointer to where the 'z' value should be stored.
 *
 * @return True on command success
 */
bool mlx_readMeasurement(float *x, float *y, float *z, struct device *mlx_device_t) {
  uint8_t tx[1] = {MLX90393_REG_RM | MLX90393_AXIS_ALL};
  uint8_t rx[6] = {0};

  /* Read a single data sample. */

  if (transceive(tx, sizeof(tx), rx, sizeof(rx), 0, mlx_device_t) != MLX90393_STATUS_OK) {  
    //return false;
  }
  

  int16_t xi, yi, zi;

  /* Convert data to uT and float. */
  xi = (rx[0] << 8) | rx[1];
  yi = (rx[2] << 8) | rx[3];
  zi = (rx[4] << 8) | rx[5];

  if (_res_x == MLX90393_RES_18)
    xi -= 0x8000;
  if (_res_x == MLX90393_RES_19)
    xi -= 0x4000;
  if (_res_y == MLX90393_RES_18)
    yi -= 0x8000;
  if (_res_y == MLX90393_RES_19)
    yi -= 0x4000;
  if (_res_z == MLX90393_RES_18)
    zi -= 0x8000;
  if (_res_z == MLX90393_RES_19)
    zi -= 0x4000;

  *x = (float)xi * mlx90393_lsb_lookup[0][_gain][_res_x][0];
  *y = (float)yi * mlx90393_lsb_lookup[0][_gain][_res_y][0];
  *z = (float)zi * mlx90393_lsb_lookup[0][_gain][_res_z][1];

  return true;
}

/** Lookup table for conversion time based on [DIF_FILT][OSR].
 */
const float mlx90393_tconv[8][4] = {
    /* DIG_FILT = 0 */
    {1.27, 1.84, 3.00, 5.30},
    /* DIG_FILT = 1 */
    {1.46, 2.23, 3.76, 6.84},
    /* DIG_FILT = 2 */
    {1.84, 3.00, 5.30, 9.91},
    /* DIG_FILT = 3 */
    {2.61, 4.53, 8.37, 16.05},
    /* DIG_FILT = 4 */
    {4.15, 7.60, 14.52, 28.34},
    /* DIG_FILT = 5 */
    {7.22, 13.75, 26.80, 52.92},
    /* DIG_FILT = 6 */
    {13.36, 26.04, 51.38, 102.07},
    /* DIF_FILT = 7 */
    {25.65, 50.61, 100.53, 200.37},
};

/**
 * Performs a single X/Y/Z conversion and returns the results.
 *
 * @param x     Pointer to where the 'x' value should be stored.
 * @param y     Pointer to where the 'y' value should be stored.
 * @param z     Pointer to where the 'z' value should be stored.
 *
 * @return True if the operation succeeded, otherwise false.
 */
bool mlx_readData(float *x, float *y, float *z, struct device *mlx_device_t) {
  if (!startSingleMeasurement(mlx_device_t)) {
    return false;
  }
  // See MLX90393 Getting Started Guide for fancy formula
  // tconv = f(OSR, DIG_FILT, OSR2, ZYXT)
  // For now, using Table 18 from datasheet
  // Without +10ms delay measurement doesn't always seem to work
  k_sleep(K_MSEC(mlx90393_tconv[_dig_filt][_osr] + 10));
	//k_sleep(K_SECONDS(1));

  return mlx_readMeasurement(x, y, z, mlx_device_t);
  return 0;
}

bool mlx_init(struct device *mlx_device) {

	//const struct device *mlx_device = device_get_binding("I2C_1");
  

  if (mlx_device == NULL) {
    return false;
  }

  if (!device_is_ready(mlx_device)) {
    return false;
  }

  if (!mlx_exitMode(mlx_device)){
    return false;
  }

  if (!mlx_reset(mlx_device)) {
    return false;
  }

  /* Set gain and sensor config. */
  if (!mlx_setGain(MLX90393_GAIN_1X, mlx_device)) {
    return false;
  }

  /* Set resolution. */
  if (!mlx_setResolution(MLX90393_X, MLX90393_RES_16, mlx_device)){
    return false;
  }
  if (!mlx_setResolution(MLX90393_Y, MLX90393_RES_16, mlx_device)){
    return false;
  }
  if (!mlx_setResolution(MLX90393_Z, MLX90393_RES_16, mlx_device)){
    return false;
  }

  /* Set oversampling. */
  if (!mlx_setOversampling(MLX90393_OSR_3, mlx_device)) {
    return false;
  }

  /* Set digital filtering. */
  if (!mlx_setFilter(MLX90393_FILTER_7, mlx_device)) {
    return false;
  }

  /* set INT pin to output interrupt */
  if (!mlx_setTrigInt(true, mlx_device)) {
    return false;
  }

  

  return true;
}