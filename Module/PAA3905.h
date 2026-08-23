#ifndef __PAA3905_H
#define __PAA3905_H


void PAA3905_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t PAA3905_ReadReg(uint8_t RegAddress);

void PAA3905_Init(void);
uint8_t PAA3905_GetID(void);
void PAA3905_GetData(int16_t *DeltaX, int16_t *DeltaY);
void PAA3905_BurstGet(int16_t *DeltaX, int16_t *DeltaY, uint8_t *Squal, uint32_t *Shutter, uint8_t *Observation);

void PAA3905_StandardDetectionSetting(void);
void PAA3905_EnhancedDetectionMode(void);

void PAA3905_AutomaticSwitchingMode012(void);
void PAA3905_AutomaticSwitchingMode01(void);
void PAA3905_ManualSwitchingMode0(void);
void PAA3905_ManualSwitchingMode1(void);
void PAA3905_ManualSwitchingMode2(void);

#endif
