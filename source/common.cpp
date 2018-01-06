//作者  蔡砚刚  20171226  xiaoc@pku.edu.cn
#include<stdio.h>
#include "common.h"
#include "mp4file.h"
#include <time.h>

#pragma warning(disable:4996)


static const int MON1[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };   //平年  
static const int MON2[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };   //闰年  
static const int YEARSECONDS[2] = { 365 * 24 * 3600, 366 * 24 * 3600 };

unsigned __int64 isleapyear(unsigned __int64 year)
{
	return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}
unsigned __int64 calyearFROM19040101(unsigned __int64 timestamp, unsigned __int64 &use_seconds)
{
	unsigned __int64 years = 1904;
	use_seconds = 0;
	while (1)
	{
		if (use_seconds + YEARSECONDS[isleapyear(years + 1)] > timestamp)
		{
			break;
		}
		else
		{
			use_seconds += YEARSECONDS[isleapyear(years + 1)];
			years++;

		}
	}
	return years;
}

unsigned __int64 calMONTHFROM19040101(unsigned __int64 year,unsigned __int64 timestamp, unsigned __int64 &use_seconds)
{
	use_seconds = 0;
	unsigned __int64 month = 0;
	for (month = 0; month < 12; month++)
	{
		unsigned __int64 current_monthseconds = 0;
		if (isleapyear(year))
		{
			current_monthseconds += MON2[month] * 24 * 3600;
		}
		else
		{
			current_monthseconds += MON1[month] * 24 * 3600;
		}
		if (use_seconds + current_monthseconds > timestamp)
		{
			break;
		}
		else
		{
			use_seconds += current_monthseconds;
		}
	}
	return month +1;
}
unsigned __int64 calDAYFROM19040101(unsigned __int64 timestamp, unsigned __int64 &use_seconds)
{
	use_seconds = 0;
	unsigned __int64 day = 1;
	unsigned __int64 current_monthseconds = 24*3600;
	while (1)
	{
		
		if (use_seconds + current_monthseconds > timestamp)
		{
			break;
		}
		else
		{
			use_seconds += current_monthseconds;
			day++;
		}
	}
	return day;
}
void printfTimeStampFROM19040101(unsigned __int64 timestamp, char *p)//打印时间戳
{
	unsigned __int64 use_seconds = 0;
	unsigned __int64 years = calyearFROM19040101(timestamp, use_seconds);
	timestamp -= use_seconds;
	unsigned __int64 months = calMONTHFROM19040101(years, timestamp, use_seconds);
	timestamp -= use_seconds;
	unsigned __int64 days = calDAYFROM19040101(timestamp, use_seconds);
	timestamp -= use_seconds;
	unsigned __int64 hour = timestamp / 3600;
	timestamp -= hour * 3600 ;
	unsigned __int64 minutes = timestamp / 60;
	timestamp -= minutes * 60;
	unsigned __int64 seconds = timestamp;
	timestamp -= seconds ;
	sprintf(p, "%04lld/%02lld/%02lld  %02lld:%02lld:%02lld", years,months,days,hour, minutes, seconds);
}

void printfTimeStamp(unsigned __int64 timestamp, char *p)//打印时间戳
{
	unsigned __int64 use_seconds = 0;
	unsigned __int64 hour = timestamp / (3600*mp4Info.timescale);
	timestamp -= hour * 3600 * mp4Info.timescale;
	unsigned __int64 minutes = timestamp / (60 *mp4Info.timescale);
	timestamp -= minutes * 60 * mp4Info.timescale;
	unsigned __int64 seconds = timestamp/mp4Info.timescale;
	timestamp -= seconds*mp4Info.timescale;
	unsigned __int64 millisecond = timestamp * 1000 / mp4Info.timescale;
	sprintf(p, "%02lld:%02lld:%02lld.%3lld", hour, minutes, seconds, millisecond);
}