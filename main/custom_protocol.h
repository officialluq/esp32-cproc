#ifndef CUSTOM_PROTOCOL__H
#define CUSTOM_PROTOCOL__H
#include <stdint.h>

// Maksymalny rozmiar payloadu
#define MAX_PAYLOAD_SIZE 255

#define CPROC_MSG_CMD 0x01
#define CPROC_MSG_RES 0x02
#define CPROC_MSG_REQ 0x03

#define CPROC_CMD_HELLO 0x01
//+------------+----------------+--------------+--------------+-----------+
//| Start Byte | Msg Type (1B)  | Length (1B)  | Payload (Xb) | CRC (16b) |
//+------------+----------------+--------------+--------------+-----------+

// Struktura ramki SPI
typedef struct {
    uint8_t start_byte;       // Start byte (np. 0xAA)
    uint8_t msg_type;         // Typ wiadomości (komenda/odpowiedź/błąd)
    uint8_t length;           // Długość payloadu
    uint8_t payload[MAX_PAYLOAD_SIZE]; // Zmienny payload
    uint16_t crc;             // CRC do weryfikacji integralności
} custom_prot_frame_t;


int parse_spi_frame(const uint8_t *data, uint8_t size, custom_prot_frame_t *frame);
int build_spi_frame(uint8_t msg_type, const uint8_t *payload, uint8_t payload_length, uint8_t *output_frame);
#endif