#ifndef __MYI2C2_H
#define __MYI2C2_H

void MyI2C2_Init(void);
void MyI2C2_Start(void);
void MyI2C2_Stop(void);
void MyI2C2_SendByte(uint8_t Byte);
uint8_t MyI2C2_ReceiveByte(void);
void MyI2C2_SendAck(uint8_t AckBit);
uint8_t MyI2C2_ReceiveAck(void);

#endif
