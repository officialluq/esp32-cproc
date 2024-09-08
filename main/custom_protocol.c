#include <stdint.h>
#include "custom_protocol.h"
#include <string.h>


static uint16_t calculate_crc(const uint8_t *data, uint8_t length) {
    uint16_t crc = 0xFFFF; 
    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001; 
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

static int build_spi_frame(uint8_t command_id, uint8_t msg_type, const uint8_t *payload, uint8_t payload_length, uint8_t *output_frame) {
    if (payload_length > MAX_PAYLOAD_SIZE) {
        return -1;
    }
    output_frame[0] = 0xAA;  
    output_frame[1] = command_id;
    output_frame[2] = msg_type;
    output_frame[3] = payload_length;
    if(payload) {
        memcpy(&output_frame[4], payload, payload_length);
    }
    uint16_t crc = calculate_crc(output_frame, 4 + payload_length);

    output_frame[4 + payload_length] = (crc >> 8) & 0xFF;
    output_frame[5 + payload_length] = crc & 0xFF;
    return 6 + payload_length;
}

// Funkcja parsująca ramkę SPI
int parse_spi_frame(const uint8_t *data, uint8_t size, custom_prot_frame_t *frame) {
    if (size < 5) {
        return -1;
    }

    frame->start_byte = data[0];
    frame->command_id = data[1];
    frame->msg_type = data[2];
    frame->length = data[3];

    if (frame->length > MAX_PAYLOAD_SIZE || size != frame->length + 5) {
        return -2;
    }

    memcpy(frame->payload, &data[4], frame->length);

    frame->crc = (data[size - 2] << 8) | data[size - 1];

    uint16_t calculated_crc = calculate_crc(data, size - 2);
    if (calculated_crc != frame->crc) {
        return -3; 
    }

    if (frame->start_byte != 0xAA) {
        return -4;
    }

    return 0;
}