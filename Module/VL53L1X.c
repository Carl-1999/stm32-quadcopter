#include <stdint.h>
#include "MyI2C2.h"
#include "VL53L1X_api.h"
#include "vl53l1_platform.h"

uint8_t VL53L1X_dev = 0x52;

void VL53L1X_Init(void)
{
	MyI2C2_Init();
	VL53L1X_SensorInit(VL53L1X_dev);
	
    VL53L1X_SetDistanceMode(VL53L1X_dev, 2); /* 1=short, 2=long */
    VL53L1X_SetTimingBudgetInMs(VL53L1X_dev, 20); /* in ms possible values [20, 50, 100, 200, 500] */
    VL53L1X_SetInterMeasurementInMs(VL53L1X_dev, 20); /* in ms, IM must be > = TB */
	
    VL53L1X_StartRanging(VL53L1X_dev);
}

uint16_t VL53L1X_GetID(void)
{
	uint16_t ID;
	VL53L1X_GetSensorId(VL53L1X_dev, &ID);
	return ID;
}

int16_t VL53L1X_GetData(void)
{
	uint8_t RangeStatus;
	uint16_t Distance;
	
	VL53L1X_GetRangeStatus(VL53L1X_dev, &RangeStatus);
	if (RangeStatus == 0)
	{
		VL53L1X_GetDistance(VL53L1X_dev, &Distance);
		return Distance;
	}
	else
	{
		return -1;
	}
	
}
