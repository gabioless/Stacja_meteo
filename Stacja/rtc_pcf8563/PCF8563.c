/*
 * PCF8563.c
 *
 * Created: 2014-08-19 13:00:51
 *  Author: tmf
 */ 

#include "I2cbase.h"
#include "PCF8563.h"


void PCF8563_WriteRegister(uint8_t reg, uint8_t value)
{
	I2C_SendStartAndSelect(PCF8563_Addr | TW_WRITE);
	I2C_SendByte(reg);
	I2C_SendByte(value);
	I2C_Stop();
}

uint8_t PCF8563_ReadRegister(uint8_t reg)
{
	I2C_SendStartAndSelect(PCF8563_Addr | TW_WRITE);
	I2C_SendByte(reg);
	I2C_SendStartAndSelect(PCF8563_Addr | TW_READ);
	uint8_t res=I2C_ReceiveData_NACK();
	I2C_Stop();
	return res;
}

uint8_t PCF8563_IsDataValid()
{
	return ((PCF8563_ReadRegister(SecondsReg) & PCF_TimeValid)==0);
}

void PCF8563_SetTime(Time *time)
{
	I2C_SendStartAndSelect(PCF8563_Addr | TW_WRITE);
	I2C_SendByte(SecondsReg);
	I2C_SendByte(time->Second);
	I2C_SendByte(time->Minute);
	I2C_SendByte(time->Hour);
	I2C_Stop();
}

void PCF8563_GetTime(Time *time)
{
	I2C_SendStartAndSelect(PCF8563_Addr | TW_WRITE);
	I2C_SendByte(SecondsReg);
	I2C_SendStartAndSelect(PCF8563_Addr | TW_READ);
	time->Second=I2C_ReceiveData_ACK();
	time->Minute=I2C_ReceiveData_ACK() & 0x7F;
	time->Hour=I2C_ReceiveData_NACK() & 0x3F;
	I2C_Stop();
}

void PCF8563_SetDate(Date *date)
{
	I2C_SendStartAndSelect(PCF8563_Addr | TW_WRITE);
	I2C_SendByte(DaysReg);
	I2C_SendByte(date->Day);
	I2C_SendByte(date->WeekDay);
	I2C_SendByte(date->Month);
	I2C_SendByte(date->Year);
	I2C_Stop();
}

void PCF8563_GetDate(Date *date)
{
	I2C_SendStartAndSelect(PCF8563_Addr | TW_WRITE);
	I2C_SendByte(DaysReg);
	I2C_SendStartAndSelect(PCF8563_Addr | TW_READ);
	date->Day=I2C_ReceiveData_ACK() & 0x3F;
	date->WeekDay=I2C_ReceiveData_ACK() & 0x07;
	date->Month=I2C_ReceiveData_ACK() & 0x1F;
	date->Year=I2C_ReceiveData_NACK();
	I2C_Stop();
}
