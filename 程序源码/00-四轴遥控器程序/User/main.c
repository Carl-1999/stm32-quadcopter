#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "Key.h"
#include "AD.h"
#include "NRF24L01.h"

uint8_t KeyNum;

uint16_t AD_LH, AD_LV, AD_RH, AD_RV;
int8_t LH, LV, RH, RV;
uint8_t KEY;

uint8_t Flag;

uint8_t SendFlag;
uint8_t SuccessRatio;

uint8_t Mode;

uint8_t MessageIndex;
uint16_t MessageCount;

int8_t DataProcess(uint16_t ADValue);
uint8_t CalculateSuccessRatio(uint8_t SendFlag);

int main(void)
{
	OLED_Init();
	NRF24L01_Init();
	Key_Init();
	AD_Init();
	
	Timer_Init();
	
	OLED_Clear();
	OLED_ShowString(0, 0,  "   [江协科技]   ", OLED_8X16);
	OLED_ShowString(0, 16, " 四轴遥控器程序 ", OLED_8X16);
	OLED_ShowString(0, 32, "      V1.0      ", OLED_8X16);
	OLED_ShowString(0, 48, "            K10>", OLED_8X16);
	OLED_Update();
	
	while (Key_GetNum() != 10);

	OLED_Clear();

	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum)
		{
			KEY = KeyNum;
		}
		if (KeyNum)
		{
			MessageIndex = KeyNum;
			MessageCount = 500;
		}
		if (KeyNum == 9)
		{
			Mode = !Mode;
			OLED_Clear();
		}
		
		AD_LH = AD_GetValue(ADC_Channel_0);
		AD_LV = AD_GetValue(ADC_Channel_1);
		AD_RH = AD_GetValue(ADC_Channel_2);
		AD_RV = AD_GetValue(ADC_Channel_3);
		
		LH = DataProcess(AD_LH);
		LV = DataProcess(AD_LV);
		RH = DataProcess(AD_RH);
		RV = DataProcess(AD_RV);
		
		if (Flag == 1)
		{
			NRF24L01_TxPacket[0] = 0x10 | Mode;
			NRF24L01_TxPacket[1] = LH;
			NRF24L01_TxPacket[2] = LV;
			NRF24L01_TxPacket[3] = RH;
			NRF24L01_TxPacket[4] = RV;
			NRF24L01_TxPacket[5] = KEY;
			
			SendFlag = NRF24L01_Send();
			
			SuccessRatio = CalculateSuccessRatio(SendFlag);
			
//			if (SendFlag == 1)
//			{
				KEY = 0;
//			}
			
			Flag = 0;
		}
		
		OLED_Clear();
		if (Mode == 0)
		{
			OLED_Printf(0, 16, OLED_8X16, "LH:%+04d", LH);
			OLED_Printf(0, 32, OLED_8X16, "LV:%+04d", LV);
			OLED_Printf(64, 16, OLED_8X16, "RH:%+04d", RH);
			OLED_Printf(64, 32, OLED_8X16, "RV:%+04d", RV);
		}
		else
		{
			static uint8_t FlyMode;
			static int16_t Throttle;
			static float BatteryVoltage, Roll, Pitch, Yaw, Height;
			if (NRF24L01_Receive() == 1)
			{
				uint8_t ID = NRF24L01_RxPacket[0];
				if (ID == 0x12)
				{
					FlyMode = NRF24L01_RxPacket[1];
					Throttle = *(int16_t *)&NRF24L01_RxPacket[2];
					BatteryVoltage = *(float *)&NRF24L01_RxPacket[4];
					Roll = *(float *)&NRF24L01_RxPacket[8];
					Pitch = *(float *)&NRF24L01_RxPacket[12];
					Yaw = *(float *)&NRF24L01_RxPacket[16];
					Height = *(float *)&NRF24L01_RxPacket[20];
					
				}
			}
			if (FlyMode == 0)
			{
				OLED_Printf(96, 0, OLED_8X16, "自稳");
			}
			else if (FlyMode == 1)
			{
				OLED_Printf(96, 0, OLED_8X16, "定高");
			}
			else if (FlyMode == 2)
			{
				OLED_Printf(96, 0, OLED_8X16, "定点");
			}
			OLED_Printf(40, 0, OLED_8X16, "%4.2fv", BatteryVoltage);
			OLED_Printf(0, 16, OLED_8X16, "R:%+04.0f  P:%+04.0f", Roll, Pitch);
			OLED_Printf(0, 32, OLED_8X16, "Y:%+04.0f  T:%+04d", Yaw, Throttle);
			OLED_Printf(0, 48, OLED_8X16, "H:%+06.0fcm ", Height);
		}
		if (SuccessRatio >= 9)
		{
			OLED_ShowImage(0, 0, 16, 16, Signal_3);
		}
		else if (SuccessRatio >= 5)
		{
			OLED_ShowImage(0, 0, 16, 16, Signal_2);
		}
		else if (SuccessRatio >= 1)
		{
			OLED_ShowImage(0, 0, 16, 16, Signal_1);
		}
		else
		{
			OLED_ShowImage(0, 0, 16, 16, Signal_0);
		}
		
		if (MessageCount > 0)
		{
			if (MessageIndex >= 1 && MessageIndex <= 8)
			{
				OLED_DrawRectangle(16, 16, 96, 32, OLED_UNFILLED);
				OLED_ClearArea(17, 17, 94, 30);
			}
			switch (MessageIndex)
			{
				case 1: OLED_Printf(48, 24, OLED_8X16, "启动"); break;
				case 2: OLED_Printf(48, 24, OLED_8X16, "停止"); break;
				case 3: OLED_Printf(28, 24, OLED_8X16, "飞行模式-"); break;
				case 4: OLED_Printf(28, 24, OLED_8X16, "飞行模式+"); break;
				case 5: OLED_Printf(32, 24, OLED_8X16, "向前微调"); break;
				case 6: OLED_Printf(32, 24, OLED_8X16, "向后微调"); break;
				case 7: OLED_Printf(32, 24, OLED_8X16, "向左微调"); break;
				case 8: OLED_Printf(32, 24, OLED_8X16, "向右微调"); break;
			}
		}
		
		OLED_Update();
	}
}

int8_t DataProcess(uint16_t ADValue)
{
	int16_t Value;
	
	Value = ADValue - 2048;
	
	//-2048 ~ 2047
	
	if (Value > 100)
	{
		Value -= 100;
	}
	else if (Value < -100)
	{
		Value += 100;
	}
	else
	{
		Value = 0;
	}
	
	//-1948 ~ 1947
	
	Value = Value * 101 / 1949;
	
	//-100 ~ 100
	
	return Value;
}

uint8_t CalculateSuccessRatio(uint8_t SendFlag)
{
	static uint8_t SendFlagArray[10];
	static uint8_t p;
	uint8_t i, SuccessCount;
	
	SendFlagArray[p] = SendFlag;
	p ++;
	p %= 10;
	
	SuccessCount = 0;
	for (i = 0; i < 10; i ++)
	{
		if (SendFlagArray[i] == 1)
		{
			SuccessCount ++;
		}
	}
	
	return SuccessCount;
}

void TIM1_UP_IRQHandler(void)
{
	static uint16_t Count;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Key_Tick();
		
		if (MessageCount > 0)
		{
			MessageCount --;
		}
		
		Count ++;
		if (Count >= 100)
		{
			Count = 0;
			
			Flag = 1;
		}
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
