/* SPI Slave example, receiver (uses SPI Slave driver to communicate with sender)

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/spi_slave.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"

#include "custom_protocol.h"
#include "command_parser.h"
#include "math.h"
#include "hw611.h"
#include "i2c_ctrl.h"
#include "spi_ctrl.h"

static const char *TAG = "MAIN.c"; 
#define GPIO_HANDSHAKE      2
#define GPIO_MOSI           12
#define GPIO_MISO           13
#define GPIO_SCLK           15
#define GPIO_CS             14
#define RCV_HOST    SPI2_HOST

#define CPROC_COMM_PRIORITY_TASK 5

/*a delay function for milliseconds delay*/
void delay_function (uint32_t delayMS)
{
  vTaskDelay(delayMS);
}


//Main application
void app_main(void)
{
    int n = 0;
    esp_err_t ret;
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;
    ESP_LOGI(TAG, "STARTING");
    ret = init_i2c(&bus_handle);
    if(ret != ESP_OK) {
        ESP_LOGE("tag", "sds");
    } 
    ret = init_spi();
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "Error occured while intializing SPI, err_code=%d", ret);
    }
    ret = init_hw611(&bus_handle, &dev_handle);
    if(ret != ESP_OK) {
        ESP_LOGE(TAG, "Error occured while intializing HW611, err_code=%d", ret);
    }
    vTaskDelay(1000);
    
    // ESP_LOGI("TAG", "TEMPERATURE: %f",sensors_value.temperature);
    xTaskCreate(communication_routine, "communication_routine", 16384, NULL, CPROC_COMM_PRIORITY_TASK, NULL);
}
