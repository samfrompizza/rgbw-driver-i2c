# rgbw-driver-i2c

## О проекте

Драйвер взаимодействия с микросхемой управления RGBW светодиодами через интерфейс I2C микроконтроллера.

Драйвер для микросхемы управления RGBW светодиодами обеспечивает следующие возможности:

- первоначальную инициализацию микросхемы управления RGBW светодиодами (все светодиоды отключены, микросхема готова к работе, микросхема всегда включена);
- возможность управления яркостью по каждому каналу по отдельности;
- включение и выключение каждого канала по отдельности;
- включение и выключения всех каналов одновременно;
- установку цвета светодиодов в градациях RGB (0 - 255 для каждого цвета) при применении единого RGB светодиода для индикации.

## Требования

- C99;
- компилятор GCC;
- CMake;
- `make`;
- подключаемый I2C API.

## Структура проекта

```text
.
├── .github/workflows     # CI
├── driver/               # логика драйвера RGBW
├── i2c-api/              # предлагаемый API I2C
├── tests/                # unit-тесты и mock
├── CMakeLists.txt        # сборка проекта
└── README.md
```

## Инструкции по сборке

Для сборки нужно запустить:

```text
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Пример использования

```code
#include "rgbw-driver.h"

int main(void)
{
    I2C_Error_t status;

    status = RGBW_Init(I2C_CH1);
    if (status != I2C_ERR_SUCCESS) {
        return -1;
    }

    status = RGBW_SetRGB(255, 0, 0);   // красный
    if (status != I2C_ERR_SUCCESS) {
        return -1;
    }

    status = RGBW_SetChannelEnable(RGBW_CH_RED, 1);
    if (status != I2C_ERR_SUCCESS) {
        return -1;
    }

    return 0;
}
```