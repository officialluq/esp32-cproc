#include "custom_protocol.h"
#include "driver/spi_slave.h"
#include "esp_log.h"
#include "string.h"
#include "custom_protocol.h"

#define RCV_HOST    SPI2_HOST
static const char* TAG = "command_parser.c";
void command_parser()
{
    esp_err_t ret;
    custom_prot_frame_t input_dataframe = {0};
    custom_prot_frame_t output_dataframe = {0};
    uint8_t input_buffer[sizeof(custom_prot_frame_t)];
    memset(&input_dataframe, 0, sizeof(input_dataframe));
    memset(&output_dataframe, 0, sizeof(output_dataframe));
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));
    ESP_LOGI(TAG, "Command Parser has been Initialized!");
    while(1) {
        t.length = sizeof(input_dataframe) * 8 * 2;
        t.tx_buffer = &output_dataframe;
        t.rx_buffer = &input_buffer;

        ret = spi_slave_transmit(RCV_HOST, &t, portMAX_DELAY);
        parse_spi_frame(input_buffer, t.length, &input_dataframe);
        ESP_LOG_BUFFER_HEXDUMP("LOG", &input_dataframe, sizeof(input_dataframe), 1);
    }
}