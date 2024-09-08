#include "esp_log.h"
#include "i2c_ctrl.h"
#include "hw611.h"
#include "math.h"
#include "string.h"


#define SCL_IO_PIN             40
#define SDA_IO_PIN             41
#define MASTER_FREQUENCY    100000
#define HW611_I2C_ADDRESS    100000
#define PORT_NUMBER -1
#define LENGTH 48

static const char *TAG = "i2c_ctrl.c";

/*implements a power function (used in altitude calculation)*/
float power_function (float x, float y)
{
    return pow(x,y);
}

esp_err_t init_hw611(i2c_master_bus_handle_t *bus_handle, i2c_master_dev_handle_t *dev_handle)
{
    esp_err_t err;
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_ADDRESS_1,
        .scl_speed_hz = MASTER_FREQUENCY,
    };

    err = i2c_master_bus_add_device(*bus_handle, &dev_cfg, dev_handle);
    if(err != ESP_OK) {
        return err;
    }
    hw611_init(dev_handle);
    return ESP_OK;
}

esp_err_t read_i2c(i2c_master_dev_handle_t *dev_handle, uint8_t *data_rd, size_t lenght)
{
    esp_err_t ret;
    ret = i2c_master_receive(*dev_handle, data_rd, lenght, -1);
    if(ret != ESP_OK) {
        return ret;
    }
    return ESP_OK;
}

esp_err_t init_i2c(i2c_master_bus_handle_t *bus_handle)
{
    esp_err_t err;
    i2c_master_bus_config_t i2c_bus_config = {
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = PORT_NUMBER,
        .scl_io_num = SCL_IO_PIN,
        .sda_io_num = SDA_IO_PIN,
    };
    err = i2c_new_master_bus(&i2c_bus_config, bus_handle);
    if(err != ESP_OK) {
        return err;
    }
    return ESP_OK;
}

void hw611_i2c_init()        
{
  
}

void hw611_read_array (void *i2c_device, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength)
{
    esp_err_t err;
    i2c_master_dev_handle_t *dev_handle = (i2c_master_dev_handle_t *)i2c_device;
    uint8_t register_address = startRegisterAddress; 
    err = i2c_master_transmit_receive(*dev_handle, &register_address, 1, data, dataLength, -1);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Read array went wrong");
    }  
}

void hw611_write_array(void *i2c_device, uint8_t startRegisterAddress, uint8_t *data, uint8_t dataLength)
{
    esp_err_t err;
    uint8_t buffer[16];
    i2c_master_dev_handle_t *dev_handle = (i2c_master_dev_handle_t *)i2c_device;
    buffer[0] = startRegisterAddress;
    memcpy(&buffer[1], data, dataLength);
    err = i2c_master_transmit(*dev_handle, buffer, dataLength + 1, 50);
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "Write array went wrong!");
    }  
}