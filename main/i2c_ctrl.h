#ifndef I2C_CTRL__H
#define I2C_CTRL__H
#include "esp_err.h"
#include "driver/i2c_master.h"

esp_err_t init_i2c(i2c_master_bus_handle_t *bus_handle);

esp_err_t init_hw611(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle);

#endif