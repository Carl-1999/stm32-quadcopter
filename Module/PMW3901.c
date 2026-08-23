#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "PMW3901.h"
#include "PMW3901_Reg.h"
#include "Delay.h"

void PMW3901_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MySPI_Start();
	MySPI_SwapByte(RegAddress | 0x80);
	MySPI_SwapByte(Data);
	MySPI_Stop();
}

uint8_t PMW3901_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	MySPI_Start();
	MySPI_SwapByte(RegAddress);
	Data = MySPI_SwapByte(0xFF);
	MySPI_Stop();
	return Data;
}

void PMW3901_Init(void)
{
	MySPI_Init();
	
	PMW3901_WriteReg(PMW3901_POWER_UP_RESET, 0x5A);
	
	PMW3901_WriteReg(0x7F, 0x00);
	PMW3901_WriteReg(0x61, 0xAD);
	PMW3901_WriteReg(0x7F, 0x03);
	PMW3901_WriteReg(0x40, 0x00);
	PMW3901_WriteReg(0x7F, 0x05);
	PMW3901_WriteReg(0x41, 0xB3);
	PMW3901_WriteReg(0x43, 0xF1);
	PMW3901_WriteReg(0x45, 0x14);
	PMW3901_WriteReg(0x5B, 0x32);
	PMW3901_WriteReg(0x5F, 0x34);
	PMW3901_WriteReg(0x7B, 0x08);
	PMW3901_WriteReg(0x7F, 0x06);
	PMW3901_WriteReg(0x44, 0x1B);
	PMW3901_WriteReg(0x40, 0xBF);
	PMW3901_WriteReg(0x4E, 0x3F);
	PMW3901_WriteReg(0x7F, 0x08);
	PMW3901_WriteReg(0x65, 0x20);
	PMW3901_WriteReg(0x6A, 0x18);
	PMW3901_WriteReg(0x7F, 0x09);
	PMW3901_WriteReg(0x4F, 0xAF);
	PMW3901_WriteReg(0x5F, 0x40);
	PMW3901_WriteReg(0x48, 0x80);
	PMW3901_WriteReg(0x49, 0x80);
	PMW3901_WriteReg(0x57, 0x77);
	PMW3901_WriteReg(0x60, 0x78);
	PMW3901_WriteReg(0x61, 0x78);
	PMW3901_WriteReg(0x62, 0x08);
	PMW3901_WriteReg(0x63, 0x50);
	PMW3901_WriteReg(0x7F, 0x0A);
	PMW3901_WriteReg(0x45, 0x60);
	PMW3901_WriteReg(0x7F, 0x00);
	PMW3901_WriteReg(0x4D, 0x11);
	PMW3901_WriteReg(0x55, 0x80);
	PMW3901_WriteReg(0x74, 0x1F);
	PMW3901_WriteReg(0x75, 0x1F);
	PMW3901_WriteReg(0x4A, 0x78);
	PMW3901_WriteReg(0x4B, 0x78);
	PMW3901_WriteReg(0x44, 0x08);
	PMW3901_WriteReg(0x45, 0x50);
	PMW3901_WriteReg(0x64, 0xFF);
	PMW3901_WriteReg(0x65, 0x1F);
	PMW3901_WriteReg(0x7F, 0x14);
	PMW3901_WriteReg(0x65, 0x67);
	PMW3901_WriteReg(0x66, 0x08);
	PMW3901_WriteReg(0x63, 0x70);
	PMW3901_WriteReg(0x7F, 0x15);
	PMW3901_WriteReg(0x48, 0x48);
	PMW3901_WriteReg(0x7F, 0x07);
	PMW3901_WriteReg(0x41, 0x0D);
	PMW3901_WriteReg(0x43, 0x14);
	PMW3901_WriteReg(0x4B, 0x0E);
	PMW3901_WriteReg(0x45, 0x0F);
	PMW3901_WriteReg(0x44, 0x42);
	PMW3901_WriteReg(0x4C, 0x80);
	PMW3901_WriteReg(0x7F, 0x10);
	PMW3901_WriteReg(0x5B, 0x02);
	PMW3901_WriteReg(0x7F, 0x07);
	PMW3901_WriteReg(0x40, 0x41);
	PMW3901_WriteReg(0x70, 0x00);

	Delay_ms(10); // delay 10ms

	PMW3901_WriteReg(0x32, 0x44);
	PMW3901_WriteReg(0x7F, 0x07);
	PMW3901_WriteReg(0x40, 0x40);
	PMW3901_WriteReg(0x7F, 0x06);
	PMW3901_WriteReg(0x62, 0xF0);
	PMW3901_WriteReg(0x63, 0x00);
	PMW3901_WriteReg(0x7F, 0x0D);
	PMW3901_WriteReg(0x48, 0xC0);
	PMW3901_WriteReg(0x6F, 0xD5);
	PMW3901_WriteReg(0x7F, 0x00);
	PMW3901_WriteReg(0x5B, 0xA0);
	PMW3901_WriteReg(0x4E, 0xA8);
	PMW3901_WriteReg(0x5A, 0x50);
	PMW3901_WriteReg(0x40, 0x80);
	
	PMW3901_WriteReg(PMW3901_POWER_UP_RESET, 0x00);
}

uint8_t PMW3901_GetID(void)
{
	return PMW3901_ReadReg(PMW3901_PRODUCT_ID);
}

void PMW3901_Get(int16_t *DeltaX, int16_t *DeltaY)
{
	uint8_t Motion, X_L, X_H, Y_L, Y_H;
	
	*DeltaX = 0;
	*DeltaY = 0;
	
	Motion = PMW3901_ReadReg(PMW3901_MOTION);
	if (Motion & 0x80)
	{
		X_L = PMW3901_ReadReg(PMW3901_DELTA_X_L);
		X_H = PMW3901_ReadReg(PMW3901_DELTA_X_H);
		Y_L = PMW3901_ReadReg(PMW3901_DELTA_Y_L);
		Y_H = PMW3901_ReadReg(PMW3901_DELTA_Y_H);
		
		*DeltaX = (X_H << 8) | X_L;
		*DeltaY = (Y_H << 8) | Y_L;
	}
}

void PMW3901_Get2(int16_t *DeltaX, int16_t *DeltaY, uint8_t *Squal, uint16_t *Shutter)
{
	uint8_t Motion, X_L, X_H, Y_L, Y_H;
	uint8_t S_L, S_H;
	
	*DeltaX = 0;
	*DeltaY = 0;
	
	Motion = PMW3901_ReadReg(PMW3901_MOTION);
	if (Motion & 0x80)
	{
		X_L = PMW3901_ReadReg(PMW3901_DELTA_X_L);
		X_H = PMW3901_ReadReg(PMW3901_DELTA_X_H);
		Y_L = PMW3901_ReadReg(PMW3901_DELTA_Y_L);
		Y_H = PMW3901_ReadReg(PMW3901_DELTA_Y_H);
		*DeltaX = (X_H << 8) | X_L;
		*DeltaY = (Y_H << 8) | Y_L;
		
		*Squal = PMW3901_ReadReg(PMW3901_SQUAL);
		
		S_L = PMW3901_ReadReg(PMW3901_SHUTTER_LOWER);
		S_H = PMW3901_ReadReg(PMW3901_SHUTTER_UPPER);
		*Shutter = (S_H << 8) | S_L;
	}
}
