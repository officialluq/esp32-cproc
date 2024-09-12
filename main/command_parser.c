#include "custom_protocol.h"
#include "driver/spi_slave.h"
#include "esp_log.h"
#include "string.h"
#include "custom_protocol.h"

#define RCV_HOST    SPI2_HOST
static const char* TAG = "command_parser.c";

typedef enum {
    HELLO = 1,
    GET_TEMP = 2,
    GET_HUMIDITY = 3,
    GET_ATTIDUTE = 4,
    LED_RED = 5,
    LED_GREEN = 6,
    LED_BLUE = 7
} custom_command_t;
typedef enum {
    COMMAND = 1,
    MSG = 2,
    RESPONSE = 3
} custom_msg_types_t;

uint8_t buffer[255];

void command_parse(custom_prot_frame_t* frame)
{
    esp_err_t ret;
    spi_slave_transaction_t message;
    int frame_len;
    custom_prot_frame_t response_frame;
    memset(&response_frame, 0, sizeof(custom_prot_frame_t));
    switch(frame->payload[0])
    {
        case HELLO:
        ESP_LOGI(TAG, "HELLO COMMAND");
        buffer[0] = HELLO;
        frame_len = build_spi_frame(COMMAND, (uint8_t*)buffer,1, &response_frame);
        message.tx_buffer = &response_frame;
        message.rx_buffer = NULL;
        message.length = frame_len * 8;
        // memset(&response_frame, 0, sizeof(custom_prot_frame_t));
        ret = spi_slave_transmit(RCV_HOST, &message, portMAX_DELAY);
        // ESP_LOG_BUFFER_HEXDUMP(TAG, &response_frame, frame_len, 1)s;
        break;
        case GET_TEMP:
        int32_t temperature = 30; // Dodac funckje od i2c 
        strcpy((char*)buffer, "30.055");
        frame_len = build_spi_frame(RESPONSE, (uint8_t*)buffer, sizeof(buffer), &response_frame);
        message.tx_buffer = &response_frame;
        message.rx_buffer = NULL;
        message.length = frame_len * 8;
        // memset(&response_frame, 0, sizeof(custom_prot_frame_t));
        // ESP_LOG_BUFFER_HEXDUMP(TAG, &response_frame, sizeof(response_frame), 1);
        ret = spi_slave_transmit(RCV_HOST, &message, portMAX_DELAY);
        ESP_LOGI(TAG,"ERR %d", ret);
        break;
        case GET_HUMIDITY:
        int32_t humidity = 30; // Dodac funckje od i2c 
        strcpy((char*)buffer, "20");
        frame_len = build_spi_frame(RESPONSE, (uint8_t*)buffer, sizeof(buffer), &response_frame);
        message.tx_buffer = &response_frame;
        message.rx_buffer = NULL;
        message.length = frame_len * 8;
        // memset(&response_frame, 0, sizeof(custom_prot_frame_t));
        // ESP_LOG_BUFFER_HEXDUMP(TAG, &response_frame, sizeof(response_frame), 1);
        ret = spi_slave_transmit(RCV_HOST, &message, portMAX_DELAY);
        ESP_LOGI(TAG,"ERR %d", ret);
        break;
        case GET_ATTIDUTE:
        int32_t attidute = 30; // Dodac funckje od i2c 
        strcpy((char*)buffer, "334");
        frame_len = build_spi_frame(RESPONSE, (uint8_t*)buffer, sizeof(buffer), &response_frame);
        message.tx_buffer = &response_frame;
        message.rx_buffer = NULL;
        message.length = frame_len * 8;
        // memset(&response_frame, 0, sizeof(custom_prot_frame_t));
        // ESP_LOG_BUFFER_HEXDUMP(TAG, &response_frame, sizeof(response_frame), 1);
        ret = spi_slave_transmit(RCV_HOST, &message, portMAX_DELAY);
        ESP_LOGI(TAG, "GOT ATTIDUTE COMMAND");
        break;
        case LED_RED:
        ESP_LOGI(TAG, "GOT RED COMMAND");
        break;
        case LED_GREEN:
        ESP_LOGI(TAG, "GOT GREEN COMMAND");
        break;
        case LED_BLUE:
        ESP_LOGI(TAG, "GOT BLUE COMMAND");
        break;
        default:
        ESP_LOGI(TAG, "Unknown command %d", frame->payload[0]);
        break;
    }
}

void process_frame(custom_prot_frame_t *frame)
{
    switch(frame->msg_type)
    {
        case COMMAND:
        command_parse(frame);
        break;
        case MSG:
        command_parse(frame);
        break;
        case RESPONSE:
        command_parse(frame);
        break;
        default:

        break;
    }
}


void communication_routine()
{
    esp_err_t ret;
    custom_prot_frame_t input_dataframe = {0};
    custom_prot_frame_t output_dataframe = {0};
    uint8_t input_buffer[sizeof(custom_prot_frame_t)];
    uint8_t output_buffer[sizeof(custom_prot_frame_t)];
    memset(&input_dataframe, 0, sizeof(input_dataframe));
    memset(&output_dataframe, 0, sizeof(output_dataframe));
    spi_slave_transaction_t t;
    ESP_LOGI(TAG, "Command Parser has been Initialized!");
    memset(&t, 0, sizeof(t));
    t.length = sizeof(input_dataframe) * 8;
    t.tx_buffer = NULL;
    t.rx_buffer = input_buffer;
    while(1) {
        memset(input_buffer, 0, sizeof(input_buffer));
        ret = spi_slave_transmit(RCV_HOST, &t, portMAX_DELAY);
        // ESP_LOG_BUFFER_HEXDUMP("LOG", input_buffer, sizeof(input_dataframe), 1);
        ret = parse_spi_frame(input_buffer, t.length, &input_dataframe);
        process_frame(&input_dataframe);
    }
}