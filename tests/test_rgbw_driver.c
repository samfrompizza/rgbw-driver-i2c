#include "../driver/rgbw-driver.h"

#include <check.h>
#include <stdlib.h>

static RGBW_Config_t make_cfg() {
  RGBW_Config_t cfg = {I2C_CH1, I2C_FREQ_400_KHZ, RGBW_I2C_BASE_ADDR};
  return cfg;
}

START_TEST(test_init_works_with_user_config) {
  RGBW_Config_t cfg = make_cfg();
  I2C_MockReset();

  ck_assert_int_eq(RGBW_Init(&cfg), I2C_ERR_SUCCESS);

  const I2C_MockState_t *st = I2C_MockGetState();
  ck_assert_uint_eq(st->init_calls, 1U);
  ck_assert_uint_eq(st->start_calls, 7U);
  ck_assert_uint_eq(st->write_calls, 7U);
  ck_assert_uint_eq(st->stop_calls, 7U);
}
END_TEST

START_TEST(test_brightness_updates_correct_pwm_register) {
  RGBW_Config_t cfg = make_cfg();
  I2C_MockReset();

  ck_assert_int_eq(RGBW_SetChannelBrightness(&cfg, RGBW_CH_D3, 0xABU), I2C_ERR_SUCCESS);

  const I2C_MockState_t *st = I2C_MockGetState();
  ck_assert_uint_eq(st->last_write_buf[0], (uint8_t)RGBW_REG_PWM_UPDATE);
  ck_assert_uint_eq(st->last_write_buf[1], 0x00U);
}
END_TEST

START_TEST(test_error_is_propagated_from_i2c_write) {
  RGBW_Config_t cfg = make_cfg();
  I2C_MockReset();
  I2C_MockSetWriteResult(I2C_ERR_NACK);

  ck_assert_int_eq(RGBW_SetAllChannelsState(&cfg, RGBW_CH_ON), I2C_ERR_NACK);

  const I2C_MockState_t *st = I2C_MockGetState();
  ck_assert_uint_eq(st->start_calls, 1U);
  ck_assert_uint_eq(st->write_calls, 1U);
  ck_assert_uint_eq(st->stop_calls, 1U);
}
END_TEST

START_TEST(test_bad_channel_is_rejected) {
  RGBW_Config_t cfg = make_cfg();
  I2C_MockReset();

  ck_assert_int_eq(RGBW_SetChannelState(&cfg, RGBW_CH_MAX_NUM, RGBW_CH_ON), I2C_ERR_BAD_PARAMS);

  const I2C_MockState_t *st = I2C_MockGetState();
  ck_assert_uint_eq(st->start_calls, 0U);
  ck_assert_uint_eq(st->write_calls, 0U);
}
END_TEST

static Suite *rgbw_driver_suite(void) {
  Suite *s = suite_create("rgbw_driver");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_init_works_with_user_config);
  tcase_add_test(tc, test_brightness_updates_correct_pwm_register);
  tcase_add_test(tc, test_error_is_propagated_from_i2c_write);
  tcase_add_test(tc, test_bad_channel_is_rejected);

  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
  int failed;
  Suite *s = rgbw_driver_suite();
  SRunner *sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}