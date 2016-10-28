#include "i2c.h"
#include "main.h"
#include "PORT.h"


void i2c_clock_start(void)
{
	TMOD &= 0x0F;
	TMOD |= 0x10;
	ET1=0;
	TH1=I2C_RELOAD_H;
	TL1=I2C_RELOAD_L;
	TF1=0;
	TR1=1;

}

void i2c_clock_delay(uint8_t control)
{
	if(TR1==1)
	{
		while(TF1==0);
	}
	TR1=0;
	if(control==1)
	{
		TH1=I2C_RELOAD_H;
		TL1=I2C_RELOAD_L;
		TF1=0;
		TR1=1;
	}

}

uint8_t i2c_write(uint8_t dev_addr,uint8_t num_bytes,uint8_t *array_name)
{
	uint8_t return_val,num_bits,send_val,send_bit,index=0;
	if((SCL==1)&&(SDA==1))
	{
		return_val=no_errors;
	}
	else
	{
		return_val=bus_busy_error;
	}
	i2c_clock_start();
	SDA=0;
	send_val=(dev_addr<<1)&0xFE;
	num_bytes++;
	do
	{
		num_bits=8;
		do
		{
			i2c_clock_delay(1);
			SCL=0;
			num_bits--;
			send_bit=((send_val>>num_bits)&0x01);
			SDA=(bit)send_bit;
			i2c_clock_delay(1);
			SCL=1;
			while(SCL!=1);
			send_bit=SDA;
			if(send_bit!=send_bit) return_val=bus_busy_error;
		}while((num_bits>0)&&return_val==0);
		if(return_val!=bus_busy_error)
		{
			i2c_clock_delay(1);
			SCL=0;
			SDA=1;
			i2c_clock_delay(1);
			SCL=1;
			while(SCL!=1);
			send_bit=SDA;
			if(send_bit!=0) return_val=NACK_error;
			send_val=array_name[index];
			index++;
			num_bytes--;
		}
		}while((num_bytes>0)&& (return_val==0));
		if(return_val!=bus_busy_error)
		{
			i2c_clock_delay(1);
			SCL=0;
			SDA=0;
			i2c_clock_delay(1);
			SCL=1;
			while(SCL!=1);
			i2c_clock_delay(1);
			SDA=1;
		}
	return return_val;
}

uint8_t i2c_read(uint8_t dev_addr,uint8_t num_bytes,uint8_t *array_name)
{
	uint8_t return_val,rcv_val,send_bit,send_val,num_bits,index=0;
		if((SCL==1)&&(SDA==1))
		{
			return_val=no_errors;
		}
		else
		{
			return_val=bus_busy_error;
		}
		i2c_clock_start();
		SDA=0;
		send_val=(dev_addr<<1)&0xFF;
		do
		{
			num_bits=8;
			do
			{
			i2c_clock_delay(1);
			SCL=0;
			SDA=1;
			i2c_clock_delay(1);
			SCL=1;
			while(SCL!=1);
			send_bit=SDA;
			rcv_val=(rcv_val<<1)|send_bit;
			}while(num_bits>0);
			array_name[index]=rcv_val;
			index++;
			if(num_bytes==1)
			{
				send_bit=1;
			}
			else
			{
				send_bit=0;
			}
			SDA=send_bit;
			num_bytes--;
		}while(num_bytes>0);
		if(return_val!=bus_busy_error)
		{
			i2c_clock_delay(1);
			SCL=0;
			SDA=0;
			i2c_clock_delay(1);
			SCL=1;
			while(SCL!=1);
			i2c_clock_delay(1);
			SDA=1;
		}
		return return_val;
}
