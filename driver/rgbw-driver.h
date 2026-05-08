#ifndef RGBW_DRIVER_H
#define RGBW_DRIVER_H

#include "../i2c-api/i2chw.h"

#define SLAVE_ADDRESS 0b01111000
#define AD_MASK 0b00000110

#define REG_SHUTDOWN 0x00
#define REG_PWM1 0x05
#define REG_PWM2 0x06
#define REG_PWM3 0x07
#define REG_PWM4 0x08
#define REG_PWM_UPDATE 0x25
#define REG_LED_CTRL1 0x2A
#define REG_LED_CTRL2 0x2B
#define REG_LED_CTRL3 0x2C
#define REG_LED_CTRL4 0x2D
#define REG_GLOBAL_CTRL 0x4A
#define REG_OUT_FREQ 0x4B
#define REG_RESET 0x4F

typedef struct {
  I2C_Ch_t i2c_ch;
  I2C_Freq_t i2c_freq;
  BYTE i2c_addr;
  I2C_Dir_t direction;
} rgbw_config_t;

typedef enum {
  OFF = (0U),
  ON = (1U),
} rgbw_ch_state_t;

I2C_Error_t rgbw_init(rgbw_config_t *dev);
I2C_Error_t rgbw_set_channel_brightness(rgbw_config_t dev, uint8_t brightness);
I2C_Error_t rgbw_channel_switch(rgbw_config_t dev, uint8_t channel_reg, rgbw_ch_state_t state);
I2C_Error_t rgbw_all_channels_switch(rgbw_config_t dev, rgbw_ch_state_t state);
I2C_Error_t rgbw_set_color(rgbw_config_t dev, uint8_t r, uint8_t g, uint8_t b, uint8_t w);

#endif