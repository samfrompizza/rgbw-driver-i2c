#include "i2chw.h"

#include <stdio.h>
#include <string.h>

static I2C_MockState_t g_mock_state;
static I2C_Error_t g_init_result = I2C_ERR_SUCCESS;
static I2C_Error_t g_start_result = I2C_ERR_SUCCESS;
static I2C_Error_t g_stop_result = I2C_ERR_SUCCESS;
static I2C_Error_t g_write_result = I2C_ERR_SUCCESS;

void I2C_MockReset(void) {
  memset(&g_mock_state, 0, sizeof(g_mock_state));
  g_init_result = I2C_ERR_SUCCESS;
  g_start_result = I2C_ERR_SUCCESS;
  g_stop_result = I2C_ERR_SUCCESS;
  g_write_result = I2C_ERR_SUCCESS;
}

void I2C_MockSetInitResult(const I2C_Error_t err) { g_init_result = err; }
void I2C_MockSetStartResult(const I2C_Error_t err) { g_start_result = err; }
void I2C_MockSetStopResult(const I2C_Error_t err) { g_stop_result = err; }
void I2C_MockSetWriteResult(const I2C_Error_t err) { g_write_result = err; }
const I2C_MockState_t *I2C_MockGetState(void) { return &g_mock_state; }

I2C_Error_t I2C_Init(const I2C_Ch_t i2c_ch, const I2C_Freq_t i2c_freq) {
  printf("I2C bus initialization.\n");
  printf("    Channel: %d\n", i2c_ch);
  printf("    Frequency: %d\n", i2c_freq);
  printf("\n");

  g_mock_state.init_calls++;
  g_mock_state.last_channel = i2c_ch;
  g_mock_state.last_freq = i2c_freq;

  return g_init_result;
}

I2C_Error_t I2C_Start(const I2C_Ch_t i2c_ch, const BYTE i2c_addr, const I2C_Dir_t direction) {
  printf("Starting I2C communication.\n");
  printf("    Channel: %d\n", i2c_ch);
  printf("    Address: %d\n", i2c_addr);
  printf("    Direction: %d\n", direction);
  printf("\n");

  g_mock_state.start_calls++;
  g_mock_state.last_channel = i2c_ch;
  g_mock_state.last_addr = i2c_addr;
  g_mock_state.last_dir = direction;

  return g_start_result;
}

I2C_Error_t I2C_Stop(const I2C_Ch_t i2c_ch) {
  printf("Stop I2C communication.\n");
  printf("    Channel: %d\n", i2c_ch);
  printf("\n");

  g_mock_state.stop_calls++;
  g_mock_state.last_channel = i2c_ch;

  return g_stop_result;
}

I2C_Error_t I2C_Read(const I2C_Ch_t i2c_ch, uint8_t *data_bytes, const uint16_t length) {
  printf("Reading %d bytes of data from channel %d.\n", length, i2c_ch);
  printf("Data:\n");
  for (int i = 0; i < length; i++) {
    printf("    %d", data_bytes[i]);
  }
  printf("\n");

  g_mock_state.read_calls++;
  g_mock_state.last_channel = i2c_ch;
  (void)data_bytes;
  (void)length;

  return I2C_ERR_SUCCESS;
}

I2C_Error_t I2C_Write(const I2C_Ch_t i2c_ch, const uint8_t *data_bytes, const uint16_t length) {
  printf("Writing %d bytes of data to channel %d.\n", length, i2c_ch);
  printf("Data:\n");
  for (int i = 0; i < length; i++) {
    printf("    %d", data_bytes[i]);
  }
  printf("\n");

  g_mock_state.write_calls++;
  g_mock_state.last_channel = i2c_ch;
  g_mock_state.last_write_len = length;
  if ((data_bytes != NULL) && (length >= 2U)) {
    g_mock_state.last_write_buf[0] = data_bytes[0];
    g_mock_state.last_write_buf[1] = data_bytes[1];
  }

  return g_write_result;
}