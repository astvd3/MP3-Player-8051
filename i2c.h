#ifndef _I2C_H
#define _I2C_H

#include "main.h"

#define I2C_FREQ 400000

#define I2C_RELOAD ((uint16_t) 65536 - (OSC_FREQ/(2*I2C_FREQ*OSC_PER_INST)))
#define I2C_RELOAD_H I2C_RELOAD/256
#define I2C_RELOAD_L I2C_RELOAD%256

void i2c_clock_start(void);
void i2c_clock_delay(uint8_t control);
uint8_t i2c_write(uint8_t dev_addr,uint8_t numbytes,uint8_t *array_name);
uint8_t i2c_read(uint8_t dev_addr,uint8_t numbytes,uint8_t *array_name);

#define no_errors 0
#define NACK_error -1
#define bus_busy_error -2

#endif
