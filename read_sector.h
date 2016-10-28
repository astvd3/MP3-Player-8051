#ifndef _read_sector_H
#define _read_sector_H

#include "main.h"

// ------ Public function prototypes -------------------------------

uint8_t Read_Sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data);

#endif