#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "PAA3905.h"
#include "PAA3905_Reg.h"
#include "Delay.h"

void PAA3905_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MySPI_Start();
	MySPI_SwapByte(RegAddress | 0x80);
	MySPI_SwapByte(Data);
	MySPI_Stop();
}

uint8_t PAA3905_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	MySPI_Start();
	MySPI_SwapByte(RegAddress);
	Data = MySPI_SwapByte(0xFF);
	MySPI_Stop();
	return Data;
}

void PAA3905_Init(void)
{
	MySPI_Init();
	
	PAA3905_WriteReg(PAA3905_POWER_UP_RESET, 0x5A);
	
	Delay_ms(1);
	
	PAA3905_ReadReg(PAA3905_MOTION);
	PAA3905_ReadReg(PAA3905_DELTA_X_L);
	PAA3905_ReadReg(PAA3905_DELTA_X_H);
	PAA3905_ReadReg(PAA3905_DELTA_Y_L);
	PAA3905_ReadReg(PAA3905_DELTA_Y_H);
	
	PAA3905_StandardDetectionSetting();
	PAA3905_AutomaticSwitchingMode012();
}

uint8_t PAA3905_GetID(void)
{
	return PAA3905_ReadReg(PAA3905_PRODUCT_ID);
}

void PAA3905_GetData(int16_t *DeltaX, int16_t *DeltaY)
{
	uint8_t Motion, X_L, X_H, Y_L, Y_H;
	
	*DeltaX = 0;
	*DeltaY = 0;
	
	Motion = PAA3905_ReadReg(PAA3905_MOTION);
	if (Motion & 0x80)
	{
		X_L = PAA3905_ReadReg(PAA3905_DELTA_X_L);
		X_H = PAA3905_ReadReg(PAA3905_DELTA_X_H);
		Y_L = PAA3905_ReadReg(PAA3905_DELTA_Y_L);
		Y_H = PAA3905_ReadReg(PAA3905_DELTA_Y_H);
		
		*DeltaX = (X_H << 8) | X_L;
		*DeltaY = (Y_H << 8) | Y_L;
	}
}

void PAA3905_BurstGet(int16_t *DeltaX, int16_t *DeltaY, uint8_t *Squal, uint32_t *Shutter, uint8_t *Observation)
{
	uint8_t BYTE[14];
	MySPI_Start();
	MySPI_SwapByte(PAA3905_MOTION_BURST);
	for (uint8_t i = 0; i < 14; i ++)
	{
		BYTE[i] = MySPI_SwapByte(0xFF);
	}
	MySPI_Stop();
	
	uint8_t Motion = BYTE[0];
	if (Motion & 0x80)
	{
		*DeltaX = (BYTE[3] << 8) | BYTE[2];
		*DeltaY = (BYTE[5] << 8) | BYTE[4];
	}
	else
	{
		*DeltaX = 0;
		*DeltaY = 0;
	}
	
	*Squal = BYTE[7];
	*Shutter = (BYTE[11] << 16) | (BYTE[12] << 8) | BYTE[13];
	*Observation = BYTE[1];
}

void PAA3905_Get2(int16_t *DeltaX, int16_t *DeltaY, uint8_t *Squal, uint32_t *Shutter, uint8_t *Mode)
{
	uint8_t Motion, X_L, X_H, Y_L, Y_H;
	uint8_t S_L, S_M, S_H;
	
	*DeltaX = 0;
	*DeltaY = 0;
	
	Motion = PAA3905_ReadReg(PAA3905_MOTION);
	if (Motion & 0x80)
	{
		X_L = PAA3905_ReadReg(PAA3905_DELTA_X_L);
		X_H = PAA3905_ReadReg(PAA3905_DELTA_X_H);
		Y_L = PAA3905_ReadReg(PAA3905_DELTA_Y_L);
		Y_H = PAA3905_ReadReg(PAA3905_DELTA_Y_H);
		*DeltaX = (X_H << 8) | X_L;
		*DeltaY = (Y_H << 8) | Y_L;
		
		*Squal = PAA3905_ReadReg(PAA3905_SQUAL);
		
		S_L = PAA3905_ReadReg(PAA3905_SHUTTER_LOWER);
		S_M = PAA3905_ReadReg(PAA3905_SHUTTER_MIDDLE);
		S_H = PAA3905_ReadReg(PAA3905_SHUTTER_UPPER);
		*Shutter = (S_H << 16) | (S_M << 8) | S_L;
		
		*Mode = PAA3905_ReadReg(PAA3905_OBSERVATION);
	}
}

void PAA3905_StandardDetectionSetting(void)
{
	PAA3905_WriteReg(0x7F, 0x00);
	PAA3905_WriteReg(0x51, 0xFF);
	PAA3905_WriteReg(0x4E, 0x2A);
	PAA3905_WriteReg(0x66, 0x3E);
	PAA3905_WriteReg(0x7F, 0x14);
	PAA3905_WriteReg(0x7E, 0x71);
	PAA3905_WriteReg(0x55, 0x00);
	PAA3905_WriteReg(0x59, 0x00);
	PAA3905_WriteReg(0x6F, 0x2C);
	PAA3905_WriteReg(0x7F, 0x05);
	PAA3905_WriteReg(0x4D, 0xAC);
	PAA3905_WriteReg(0x4E, 0x32);
	PAA3905_WriteReg(0x7F, 0x09);
	PAA3905_WriteReg(0x5C, 0xAF);
	PAA3905_WriteReg(0x5F, 0xAF);
	PAA3905_WriteReg(0x70, 0x08);
	PAA3905_WriteReg(0x71, 0x04);
	PAA3905_WriteReg(0x72, 0x06);
	PAA3905_WriteReg(0x74, 0x3C);
	PAA3905_WriteReg(0x75, 0x28);
	PAA3905_WriteReg(0x76, 0x20);
	PAA3905_WriteReg(0x4E, 0xBF);
	PAA3905_WriteReg(0x7F, 0x03);
	PAA3905_WriteReg(0x64, 0x14);
	PAA3905_WriteReg(0x65, 0x0A);
	PAA3905_WriteReg(0x66, 0x10);
	PAA3905_WriteReg(0x55, 0x3C);
	PAA3905_WriteReg(0x56, 0x28);
	PAA3905_WriteReg(0x57, 0x20);
	PAA3905_WriteReg(0x4A, 0x2D);

	PAA3905_WriteReg(0x4B, 0x2D);
	PAA3905_WriteReg(0x4E, 0x4B);
	PAA3905_WriteReg(0x69, 0xFA);
	PAA3905_WriteReg(0x7F, 0x05);
	PAA3905_WriteReg(0x69, 0x1F);
	PAA3905_WriteReg(0x47, 0x1F);
	PAA3905_WriteReg(0x48, 0x0C);
	PAA3905_WriteReg(0x5A, 0x20);
	PAA3905_WriteReg(0x75, 0x0F);
	PAA3905_WriteReg(0x4A, 0x0F);
	PAA3905_WriteReg(0x42, 0x02);
	PAA3905_WriteReg(0x45, 0x03);
	PAA3905_WriteReg(0x65, 0x00);
	PAA3905_WriteReg(0x67, 0x76);
	PAA3905_WriteReg(0x68, 0x76);
	PAA3905_WriteReg(0x6A, 0xC5);
	PAA3905_WriteReg(0x43, 0x00);
	PAA3905_WriteReg(0x7F, 0x06);
	PAA3905_WriteReg(0x4A, 0x18);
	PAA3905_WriteReg(0x4B, 0x0C);
	PAA3905_WriteReg(0x4C, 0x0C);
	PAA3905_WriteReg(0x4D, 0x0C);
	PAA3905_WriteReg(0x46, 0x0A);
	PAA3905_WriteReg(0x59, 0xCD);
	PAA3905_WriteReg(0x7F, 0x0A);
	PAA3905_WriteReg(0x4A, 0x2A);
	PAA3905_WriteReg(0x48, 0x96);
	PAA3905_WriteReg(0x52, 0xB4);
	PAA3905_WriteReg(0x7F, 0x00);
	PAA3905_WriteReg(0x5B, 0xA0);
}

void PAA3905_EnhancedDetectionMode(void)
{
	PAA3905_WriteReg(0x7F, 0x00);
	PAA3905_WriteReg(0x51, 0xFF);
	PAA3905_WriteReg(0x4E, 0x2A);
	PAA3905_WriteReg(0x66, 0x26);
	PAA3905_WriteReg(0x7F, 0x14);
	PAA3905_WriteReg(0x7E, 0x71);
	PAA3905_WriteReg(0x55, 0x00);
	PAA3905_WriteReg(0x59, 0x00);
	PAA3905_WriteReg(0x6F, 0x2C);
	PAA3905_WriteReg(0x7F, 0x05);
	PAA3905_WriteReg(0x4D, 0xAC);
	PAA3905_WriteReg(0x4E, 0x65);
	PAA3905_WriteReg(0x7F, 0x09);
	PAA3905_WriteReg(0x5C, 0xAF);
	PAA3905_WriteReg(0x5F, 0xAF);
	PAA3905_WriteReg(0x70, 0x00);
	PAA3905_WriteReg(0x71, 0x00);
	PAA3905_WriteReg(0x72, 0x00);
	PAA3905_WriteReg(0x74, 0x14);
	PAA3905_WriteReg(0x75, 0x14);
	PAA3905_WriteReg(0x76, 0x06);
	PAA3905_WriteReg(0x4E, 0x8F);
	PAA3905_WriteReg(0x7F, 0x03);
	PAA3905_WriteReg(0x64, 0x00);
	PAA3905_WriteReg(0x65, 0x00);
	PAA3905_WriteReg(0x66, 0x00);
	PAA3905_WriteReg(0x55, 0x14);
	PAA3905_WriteReg(0x56, 0x14);
	PAA3905_WriteReg(0x57, 0x06);
	PAA3905_WriteReg(0x4A, 0x20);

	PAA3905_WriteReg(0x4B, 0x20);
	PAA3905_WriteReg(0x4E, 0x32);
	PAA3905_WriteReg(0x69, 0xFE);
	PAA3905_WriteReg(0x7F, 0x05);
	PAA3905_WriteReg(0x69, 0x14);
	PAA3905_WriteReg(0x47, 0x14);
	PAA3905_WriteReg(0x48, 0x1C);
	PAA3905_WriteReg(0x5A, 0x20);
	PAA3905_WriteReg(0x75, 0xE5);
	PAA3905_WriteReg(0x4A, 0x05);
	PAA3905_WriteReg(0x42, 0x04);
	PAA3905_WriteReg(0x45, 0x03);
	PAA3905_WriteReg(0x65, 0x00);
	PAA3905_WriteReg(0x67, 0x50);
	PAA3905_WriteReg(0x68, 0x50);
	PAA3905_WriteReg(0x6A, 0xC5);
	PAA3905_WriteReg(0x43, 0x00);
	PAA3905_WriteReg(0x7F, 0x06);
	PAA3905_WriteReg(0x4A, 0x1E);
	PAA3905_WriteReg(0x4B, 0x1E);
	PAA3905_WriteReg(0x4C, 0x34);
	PAA3905_WriteReg(0x4D, 0x34);
	PAA3905_WriteReg(0x46, 0x32);
	PAA3905_WriteReg(0x59, 0x0D);
	PAA3905_WriteReg(0x7F, 0x0A);
	PAA3905_WriteReg(0x4A, 0x2A);
	PAA3905_WriteReg(0x48, 0x96);
	PAA3905_WriteReg(0x52, 0xB4);
	PAA3905_WriteReg(0x7F, 0x00);
	PAA3905_WriteReg(0x5B, 0xA0);
}

void PAA3905_AutomaticSwitchingMode012(void)
{
	PAA3905_WriteReg(0x7F, 0x08);
	PAA3905_WriteReg(0x68, 0x02);
	PAA3905_WriteReg(0x7F, 0x00);
}

void PAA3905_AutomaticSwitchingMode01(void)
{
	PAA3905_WriteReg(0x7F, 0x08);
	PAA3905_WriteReg(0x68, 0x01);
	PAA3905_WriteReg(0x7F, 0x00);
}

void PAA3905_ManualSwitchingMode0(void)
{
	PAA3905_WriteReg(0x7F, 0x08);
	PAA3905_WriteReg(0x68, 0x00);
	PAA3905_WriteReg(0x7F, 0x00);
}

void PAA3905_ManualSwitchingMode1(void)
{
	PAA3905_WriteReg(0x7F, 0x08);
	PAA3905_WriteReg(0x68, 0x11);
	PAA3905_WriteReg(0x7F, 0x00);
}

void PAA3905_ManualSwitchingMode2(void)
{
	PAA3905_WriteReg(0x7F, 0x08);
	PAA3905_WriteReg(0x68, 0x22);
	PAA3905_WriteReg(0x7F, 0x00);
}
