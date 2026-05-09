#include "rgbw-driver.h"

#include <stddef.h>

#define RGBW_NORMAL_OPERATION (0x01U)
#define RGBW_SOFTWARE_SHUTDOWN (0x00U)
#define RGBW_GLOBAL_ENABLE_ALL (0x00U)
#define RGBW_GLOBAL_SHUTDOWN_ALL (0x01U)
#define RGBW_IMAX_FULL_SCALE (0x00U)

static I2C_Error_t rgbw_write_register(const RGBW_Config_t *dev, const uint8_t reg, const uint8_t value) {
  I2C_Error_t err;
  uint8_t frame[2] = {reg, value};

  err = I2C_Start(dev->i2c_ch, dev->i2c_addr, I2C_DIR_WRITE);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }

  err = I2C_Write(dev->i2c_ch, frame, (uint16_t)2U);
  if (err != I2C_ERR_SUCCESS) {
    (void)I2C_Stop(dev->i2c_ch);
    return err;
  }

  err = I2C_Stop(dev->i2c_ch);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }

  return I2C_ERR_SUCCESS;
}

static I2C_Error_t rgbw_apply_updates(const RGBW_Config_t *dev) {
  return rgbw_write_register(dev, (uint8_t)RGBW_REG_PWM_UPDATE, 0x00U);
}

static I2C_Error_t rgbw_validate_config(const RGBW_Config_t *dev) {
  if (dev == NULL) {
    return I2C_ERR_BAD_PARAMS;
  }
  if (dev->i2c_ch >= I2C_CH_MAX_NUM) {
    return I2C_ERR_BAD_PARAMS;
  }
  if (dev->i2c_freq >= I2C_FREQ_MAX_NUM) {
    return I2C_ERR_BAD_PARAMS;
  }
  return I2C_ERR_SUCCESS;
}

I2C_Error_t RGBW_Init(const RGBW_Config_t *dev) {
  I2C_Error_t err = rgbw_validate_config(dev);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }

  err = I2C_Init(dev->i2c_ch, dev->i2c_freq);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }

  err = rgbw_write_register(dev, (uint8_t)RGBW_REG_SHUTDOWN, RGBW_NORMAL_OPERATION);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }

  err = rgbw_write_register(dev, (uint8_t)RGBW_REG_GLOBAL_CTRL, RGBW_GLOBAL_SHUTDOWN_ALL);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }

  err = rgbw_write_register(dev, (uint8_t)RGBW_REG_LED_CTRL1, RGBW_IMAX_FULL_SCALE);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }
  err = rgbw_write_register(dev, (uint8_t)RGBW_REG_LED_CTRL2, RGBW_IMAX_FULL_SCALE);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }
  err = rgbw_write_register(dev, (uint8_t)RGBW_REG_LED_CTRL3, RGBW_IMAX_FULL_SCALE);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }
  err = rgbw_write_register(dev, (uint8_t)RGBW_REG_LED_CTRL4, RGBW_IMAX_FULL_SCALE);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }

  return rgbw_apply_updates(dev);
}

I2C_Error_t RGBW_SetChannelBrightness(const RGBW_Config_t *dev, const RGBW_Channel_t channel, const uint8_t brightness) {
  I2C_Error_t err = rgbw_validate_config(dev);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }
  if (channel >= RGBW_CH_MAX_NUM) {
    return I2C_ERR_BAD_PARAMS;
  }

  err = rgbw_write_register(dev, (uint8_t)((uint8_t)RGBW_REG_PWM1 + (uint8_t)channel), brightness);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }

  return rgbw_apply_updates(dev);
}

I2C_Error_t RGBW_SetChannelState(const RGBW_Config_t *dev, const RGBW_Channel_t channel, const RGBW_ChannelState_t state) {
  I2C_Error_t err = rgbw_validate_config(dev);

  if (err != I2C_ERR_SUCCESS) {
    return err;
  }
  if ((channel >= RGBW_CH_MAX_NUM) || (state > RGBW_CH_ON)) {
    return I2C_ERR_BAD_PARAMS;
  }

  err = rgbw_write_register(dev, (uint8_t)((uint8_t)RGBW_REG_LED_CTRL1 + (uint8_t)channel), (uint8_t)state);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }

  return rgbw_apply_updates(dev);
}

I2C_Error_t RGBW_SetAllChannelsState(const RGBW_Config_t *dev, const RGBW_ChannelState_t state) {
  I2C_Error_t err = rgbw_validate_config(dev);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }
  if (state > RGBW_CH_ON) {
    return I2C_ERR_BAD_PARAMS;
  }

  if (state == RGBW_CH_ON) {
    return rgbw_write_register(dev, (uint8_t)RGBW_REG_GLOBAL_CTRL, RGBW_GLOBAL_ENABLE_ALL);
  }

  return rgbw_write_register(dev, (uint8_t)RGBW_REG_GLOBAL_CTRL, RGBW_GLOBAL_SHUTDOWN_ALL);
}

I2C_Error_t RGBW_SetColor(const RGBW_Config_t *dev, const uint8_t r, const uint8_t g, const uint8_t b) {
  I2C_Error_t err;

  err = RGBW_SetChannelBrightness(dev, RGBW_CH_D1, r);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }
  err = RGBW_SetChannelBrightness(dev, RGBW_CH_D2, g);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }
  err = RGBW_SetChannelBrightness(dev, RGBW_CH_D3, b);
  if (err != I2C_ERR_SUCCESS) {
    return err;
  }

  return I2C_ERR_SUCCESS;
}