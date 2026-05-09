#ifndef RGBW_DRIVER_H
#define RGBW_DRIVER_H

#include "../i2c-api/i2chw.h"

#define RGBW_I2C_BASE_ADDR (0x3CU)
#define RGBW_I2C_AD_BITS_MASK (0x03U)

#define RGBW_I2C_ADDR_MIN (RGBW_I2C_BASE_ADDR)
#define RGBW_I2C_ADDR_MAX (RGBW_I2C_BASE_ADDR | RGBW_I2C_AD_BITS_MASK)

/** @brief Driver's register map. */
typedef enum {
  RGBW_REG_SHUTDOWN = (0x00U),
  RGBW_REG_PWM1 = (0x05U),
  RGBW_REG_PWM2 = (0x06U),
  RGBW_REG_PWM3 = (0x07U),
  RGBW_REG_PWM4 = (0x08U),
  RGBW_REG_PWM_UPDATE = (0x25U),
  RGBW_REG_LED_CTRL1 = (0x2AU),
  RGBW_REG_LED_CTRL2 = (0x2BU),
  RGBW_REG_LED_CTRL3 = (0x2CU),
  RGBW_REG_LED_CTRL4 = (0x2DU),
  RGBW_REG_GLOBAL_CTRL = (0x4AU),
  RGBW_REG_OUT_FREQ = (0x4BU),
  RGBW_REG_RESET = (0x4FU)
} RGBW_Reg_t;

/** @brief Avalible I2C channels. */
typedef enum {
  RGBW_CH_D1 = (0U),
  RGBW_CH_D2 = (1U),
  RGBW_CH_D3 = (2U),
  RGBW_CH_D4 = (3U),
  RGBW_CH_MAX_NUM = (4U)
} RGBW_Channel_t;

/** @brief LED's status. */
typedef enum {
  RGBW_CH_OFF = (0U),
  RGBW_CH_ON = (1U)
} RGBW_ChannelState_t;

/** @brief Structure to configure I2C device. */
typedef struct {
  I2C_Ch_t i2c_ch;
  I2C_Freq_t i2c_freq;
  BYTE i2c_addr;
} RGBW_Config_t;

/**
 * @brief Initializes the RGBW driver with user's config.
 * @param dev Driver's configuration.
 * @return I2C_Error_t Status of the operation.
 */
I2C_Error_t RGBW_Init(const RGBW_Config_t *dev);

/**
 * @brief Sets selected LED's brightness.
 * @param dev Driver's configuration.
 * @param channel LED's channel.
 * @param brightness A number from 0 to 255.
 * @return I2C_Error_t Status of the operation.
 */
I2C_Error_t RGBW_SetChannelBrightness(const RGBW_Config_t *dev, RGBW_Channel_t channel, uint8_t brightness);

/**
 * @brief Turns ON/OFF selected LED.
 * @param dev Driver's configuration.
 * @param channel LED's channel.
 * @param state ON/OFF status of LED.
 * @return I2C_Error_t Status of the operation.
 */
I2C_Error_t RGBW_SetChannelState(const RGBW_Config_t *dev, RGBW_Channel_t channel, RGBW_ChannelState_t state);

/**
 * @brief Turns ON/OFF all LEDs.
 * @param dev Driver's configuration.
 * @param state ON/OFF status of LED.
 * @return I2C_Error_t Status of the operation.
 */
I2C_Error_t RGBW_SetAllChannelsState(const RGBW_Config_t *dev, RGBW_ChannelState_t state);

/**
 * @brief Sets color by setting RGB channels.
 * @param dev Driver's configuration.
 * @param r Red channel brightness.
 * @param g Green channel brightness.
 * @param b Blue channel brightness.
 * @return I2C_Error_t Status of the operation.
 */
I2C_Error_t RGBW_SetColor(const RGBW_Config_t *dev, uint8_t r, uint8_t g, uint8_t b);

#endif