# Pedometer

Project for the class "Methodology of System Design and Modeling 1"

## 1. Hardware
- NRF52840 DK
- LSM6DSO IMU

## 2. Functionalities
- The MCU gather data (acceleration, angular rate, temperature and steps) from the IMU.
- The raw data is send via BLE protocol when MCU is connected to another device. Each data type has its own own GATT characteristic.

## 3. TODO list
### Software
- [x] GPIO driver (abstraction layer)
- [x] LSM6DSO IMU driver
- [x] SPI driver (abstraction layer)
- [x] BLE (abstraction layer)
- [x] Pedometer(abstraction layer)
### Others
- [x] Comments in the software
- [x] Documentation
