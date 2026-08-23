#ifndef __PMW3901_H
#define __PMW3901_H

void PMW3901_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t PMW3901_ReadReg(uint8_t RegAddress);

void PMW3901_Init(void);
uint8_t PMW3901_GetID(void);
void PMW3901_Get(int16_t *DeltaX, int16_t *DeltaY);
void PMW3901_Get2(int16_t *DeltaX, int16_t *DeltaY, uint8_t *Squal, uint16_t *Shutter);

#endif
