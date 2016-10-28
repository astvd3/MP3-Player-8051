#ifndef _SPI_H
#define _SPI_H

#include "main.h"

#define TIMEOUT_ERROR 4

uint8_t SPI_Master_Init(uint32_t clock_rate);
uint8_t SPI_Transfer(uint8_t send_value, uint8_t *received_value);

#endif