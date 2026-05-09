#include "../driver/rgbw-driver.h"

#include <assert.h>
#include <stdio.h>

static RGBW_Config_t make_cfg() {
  RGBW_Config_t cfg = {I2C_CH1, I2C_FREQ_400_KHZ, RGBW_I2C_BASE_ADDR};
  return cfg;
}

static void test_init_works_with_user_config() {
  RGBW_Config_t cfg = make_cfg();
  I2C_MockReset();

  assert(RGBW_Init(&cfg) == I2C_ERR_SUCCESS);

  const I2C_MockState_t *st = I2C_MockGetState();
  assert(st->init_calls == 1U);
  assert(st->start_calls == 7U);
  assert(st->write_calls == 7U);
  assert(st->stop_calls == 7U);
}

static void test_brightness_updates_correct_pwm_register() {
  RGBW_Config_t cfg = make_cfg();
  I2C_MockReset();

  assert(RGBW_SetChannelBrightness(&cfg, RGBW_CH_D3, 0xABU) == I2C_ERR_SUCCESS);

  const I2C_MockState_t *st = I2C_MockGetState();
  assert(st->last_write_buf[0] == (uint8_t)RGBW_REG_PWM_UPDATE);
  assert(st->last_write_buf[1] == 0x00U);
}

static void test_error_is_propagated_from_i2c_write() {
  RGBW_Config_t cfg = make_cfg();
  I2C_MockReset();
  I2C_MockSetWriteResult(I2C_ERR_NACK);

  assert(RGBW_SetAllChannelsState(&cfg, RGBW_CH_ON) == I2C_ERR_NACK);

  const I2C_MockState_t *st = I2C_MockGetState();
  assert(st->start_calls == 1U);
  assert(st->write_calls == 1U);
  assert(st->stop_calls == 1U);
}

static void test_bad_channel_is_rejected() {
  RGBW_Config_t cfg = make_cfg();
  I2C_MockReset();

  assert(RGBW_SetChannelState(&cfg, RGBW_CH_MAX_NUM, RGBW_CH_ON) == I2C_ERR_BAD_PARAMS);

  const I2C_MockState_t *st = I2C_MockGetState();
  assert(st->start_calls == 0U);
  assert(st->write_calls == 0U);
}

int main(void) {
  test_init_works_with_user_config();
  test_brightness_updates_correct_pwm_register();
  test_error_is_propagated_from_i2c_write();
  test_bad_channel_is_rejected();

  printf("All RGBW driver tests passed.\n");
  return 0;
}