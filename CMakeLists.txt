# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(Pedometer_nRF)

target_sources(app PRIVATE App/main.c
                           App/BLE/Src/ble.c
                           Drivers/Gpio/Src/gpio.c
                           Drivers/Spi/Src/spi.c
                           Drivers/LSM6DSO/Src/lsm6dso.c
                           Drivers/LSM6DSO/Src/lsm6dso_com.c
                           Drivers/LSM6DSO/Src/lsm6dso_tasks.c
                           Drivers/LSM6DSO/Cfg/lsm6dso_cb.c
                           HAL/Pedometer/Src/pedometer.c
                           )

target_include_directories(app PRIVATE
  App/
  App/BLE/Src/
  Drivers/Gpio/Src/
  Drivers/Spi/Src/
  Drivers/Spi/Cfg/
  Drivers/LSM6DSO/Src/
  Drivers/LSM6DSO/Cfg/
  HAL/Pedometer/Src/
  )


