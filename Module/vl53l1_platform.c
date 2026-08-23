
/**
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "vl53l1_platform.h"
#include <string.h>
#include <time.h>
#include <math.h>

#include "MyI2C2.h"
#include "Delay.h"

int8_t VL53L1_WriteMulti( uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
	MyI2C2_Start();
	MyI2C2_SendByte(dev);
	MyI2C2_ReceiveAck();
	MyI2C2_SendByte(index >> 8);
	MyI2C2_ReceiveAck();
	MyI2C2_SendByte(index & 0xFF);
	MyI2C2_ReceiveAck();
	for (uint32_t i = 0; i < count; i ++)
	{
		MyI2C2_SendByte(pdata[i]);
		MyI2C2_ReceiveAck();
	}
	MyI2C2_Stop();
		
	return 0;
}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count){
	MyI2C2_Start();
	MyI2C2_SendByte(dev);
	MyI2C2_ReceiveAck();
	MyI2C2_SendByte(index >> 8);
	MyI2C2_ReceiveAck();
	MyI2C2_SendByte(index & 0xFF);
	MyI2C2_ReceiveAck();
	
	MyI2C2_Start();
	MyI2C2_SendByte(dev | 0x01);
	MyI2C2_ReceiveAck();
	for (uint32_t i = 0; i < count; i ++)
	{
		pdata[i] = MyI2C2_ReceiveByte();
		if (i < count - 1)
		{
			MyI2C2_SendAck(0);
		}
		else
		{
			MyI2C2_SendAck(1);
		}
	}
	MyI2C2_Stop();
	
	return 0;
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
		
	VL53L1_WriteMulti(dev, index, &data, 1);
	
	return 0;
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
	
	uint8_t buf[2];
	
	buf[0] = data >> 8;
	buf[1] = data & 0xFF;
	
	VL53L1_WriteMulti(dev, index, buf, 2);
	
	return 0;
}

int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
	uint8_t buf[4];
	
	buf[0] = (data >> 24) & 0xFF;
	buf[1] = (data >> 16) & 0xFF;
	buf[2] = (data >> 8) & 0xFF;
	buf[3] = data & 0xFF;
	
	VL53L1_WriteMulti(dev, index, buf, 4);
	return 0;
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
	
	VL53L1_ReadMulti(dev, index, data, 1);
	
	return 0;
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
	
	uint8_t buf[2];
	
	VL53L1_ReadMulti(dev, index, buf, 2);
	
	*data = (buf[0] << 8) | buf[1];
	
	return 0;
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
	uint8_t buf[4];
	
	VL53L1_ReadMulti(dev, index, buf, 4);
	
	*data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
	
	return 0;
}

int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms){
	
	Delay_ms(wait_ms);
	
	return 0;
}
