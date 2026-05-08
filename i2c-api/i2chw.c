#include "i2chw.h"

#include <stdio.h>

I2C_Error_t I2C_Init(const I2C_Ch_t i2c_ch, const I2C_Freq_t i2c_freq) {
  printf("I2C bus initialization.\n");
  printf("    Channel: %d\n", i2c_ch);
  printf("    Frequency: %d\n", i2c_freq);
  printf("\n");

  return I2C_ERR_SUCCESS;
}

I2C_Error_t I2C_Start(const I2C_Ch_t i2c_ch, const BYTE i2c_addr, const I2C_Dir_t direction) {
  printf("Starting I2C communication.\n");
  printf("    Channel: %d\n", i2c_ch);
  printf("    Address: %d\n", i2c_addr);
  printf("    Direction: %d\n", direction);
  printf("\n");

  return I2C_ERR_SUCCESS;
}

I2C_Error_t I2C_Stop(const I2C_Ch_t i2c_ch) {
  printf("Stop I2C communication.\n");
  printf("    Channel: %d\n", i2c_ch);
  printf("\n");

  return I2C_ERR_SUCCESS;
}

I2C_Error_t I2C_Read(const I2C_Ch_t i2c_ch, uint8_t *data_bytes, const uint16_t length) {
  printf("Reading %d bytes of data from channel %d.\n", length, i2c_ch);
  printf("Data:\n");
  for (int i = 0; i < length; i++) {
    printf("    %d", data_bytes[i]);
  }
  printf("\n");

  return I2C_ERR_SUCCESS;
}

I2C_Error_t I2C_Write(const I2C_Ch_t i2c_ch, const uint8_t *data_bytes, const uint16_t length) {
  printf("Writing %d bytes of data to channel %d.\n", length, i2c_ch);
  printf("Data:\n");
  for (int i = 0; i < length; i++) {
    printf("    %d", data_bytes[i]);
  }
  printf("\n");

  return I2C_ERR_SUCCESS;
}