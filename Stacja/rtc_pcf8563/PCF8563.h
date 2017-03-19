/*
 * PCF8563.h
 *
 * Created: 2014-08-19 12:57:46
 *  Author: tmf
 */ 


#ifndef PCF8563_H_
#define PCF8563_H_

enum Registers {CntrlReg1=0, CntrlReg2=1, SecondsReg=2, MinutesReg=3, HoursReg=4, DaysReg=5, WeekDaysReg=6, MonthsReg=7, YearsReg=8, MinuteAlarmReg=9, HourAlarmReg=10, DayAlarmReg=11, WeekDayAlarmReg=12, CLKUOUTFreqReg=13, TimerCntrlReg=14, TimerCountDownReg=15};
enum Alarm {AlarmDisable=128};
enum Flags {PCF_TimeValid=128, PCF_CLKOUTActivate=128, PCF_CLKOUT1Hz=3, PCF_CLKOUT32Hz=2, PCF_CLKOUT1024Hz=1, PCF_CLKOUT32768Hz=0, PCF_EnableTimer=128, PCF_TimerClk4096Hz=0, PCF_TimerClk64Hz=1, PCF_TimerClk1Hz=2, PCF_TimerClk160sHz=3, PCF_AlarmInterruptEnabled=2, PCF_TimerInterruptEnabled=1};
enum Days {PCF_Sunday, PCF_Monday, PCF_Tuesday, PCF_Wednesday, PCF_Thursday, PCF_Friday, PCF_Saturday};
enum Months {PCF_January=1, PCF_February, PCF_March, PCF_April, PCF_May, PCF_June, PCF_July, PCF_August, PCF_September, PCF_October, PCF_November, PCF_December};

typedef struct
{
	uint8_t Day;
	uint8_t WeekDay;
	uint8_t Month;
	uint8_t Year;
} Date;

typedef struct
{
	uint8_t Second;
	uint8_t Minute;
	uint8_t Hour;
} Time;

#define PCF8563_Addr 0xA2    //Adres uk³adu RTC

static inline uint8_t bcd2bin(uint8_t n) { return ((((n >> 4) & 0x0F) * 10) + (n & 0x0F)); };
static inline uint8_t bin2bcd(uint8_t n) { return (((n / 10) << 4) | (n % 10)); };
static inline void bcd2ASCII(uint8_t bcd, char *ascii) {ascii[0]='0'+(bcd>>4); ascii[1]='0'+(bcd & 0x0F);};

void PCF8563_WriteRegister(uint8_t reg, uint8_t value);
uint8_t PCF8563_ReadRegister(uint8_t reg);
uint8_t PCF8563_IsDataValid();
void PCF8563_SetTime(Time *time);
void PCF8563_GetTime(Time *time);
void PCF8563_SetDate(Date *date);
void PCF8563_GetDate(Date *date);

#endif /* PCF8563_H_ */