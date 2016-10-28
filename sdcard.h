#ifndef _SDCARD_H
#define _SDCARD_H

#include "main.h"

#define no_errors 0
#define SPI_error 1
#define illegal_command 2
#define SD_CARD_ERROR 3
#define DATA_ERROR_TOKEN 5
#define voltage_error 6
#define invalid_echopattern 7

#define Type1 0  //Version 1 SD Card
#define Type2 1	 //Version 2 SD Card


uint8_t send_command(uint8_t command, uint32_t argument);
uint8_t receive_response(uint8_t num_of_bytes, uint8_t *array_out);
uint8_t SDCard_Init(void);
uint8_t read_block(uint16_t num_of_bytes, uint8_t *array);

#endif