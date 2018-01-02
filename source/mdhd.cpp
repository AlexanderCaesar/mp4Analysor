//作者  蔡砚刚  20171227  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "mdhd.h"
#include "common.h"
#include <string.h>

#pragma warning(disable:4996)

MDHDBOX::MDHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

}
int MDHDBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++MDHDBOX 概要信息+++++++++++++++++++++\n");
	unsigned char data[128];
	int ret = fread(&version, 1, 1, g_mp4_file);

	if (ret !=1)
	{
		fprintf(g_mp4_log, "错误:BOX version不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "version          :    %d\n", version);

	ret = fread(data, 1, 3, g_mp4_file);
	if (ret !=3)
	{
		fprintf(g_mp4_log, "错误:flag不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	flag = (((unsigned int)data[0]) << 16) + (((unsigned int)data[1]) << 8) + (((unsigned int)data[2]) << 0);
	fprintf(g_mp4_log, "flag             :    %u\n", flag);
	
	read_bytes += 4;
	if (version == 1)
	{
		ret = fread(data, 1, 8, g_mp4_file);
		if (ret !=8)
		{
			fprintf(g_mp4_log, "错误:creation_time不够8字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		creation_time = (((unsigned __int64)data[0]) << 56) + (((unsigned __int64)data[1]) << 48) + (((unsigned __int64)data[2]) << 40) + (((unsigned __int64)data[3]) << 32) + (((unsigned __int64)data[4]) << 24) + (((unsigned __int64)data[5]) << 16) + (((unsigned __int64)data[6]) << 8) + (((unsigned __int64)data[7]) << 0);
		char stamp_tmep[128];
		printfTimeStampFROM19040101(creation_time, stamp_tmep);
		fprintf(g_mp4_log, "creation_time    :    %lld (%s) 文件创建时间 时间戳从1904.1.1开始计算\n", creation_time, stamp_tmep);

		ret = fread(data, 1, 8, g_mp4_file);
		if (ret !=8)
		{
			fprintf(g_mp4_log, "错误:modification_time不够8字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		modification_time = (((unsigned __int64)data[0]) << 56) + (((unsigned __int64)data[1]) << 48) + (((unsigned __int64)data[2]) << 40) + (((unsigned __int64)data[3]) << 32) + (((unsigned __int64)data[4]) << 24) + (((unsigned __int64)data[5]) << 16) + (((unsigned __int64)data[6]) << 8) + (((unsigned __int64)data[7]) << 0);
		printfTimeStampFROM19040101(modification_time, stamp_tmep);
		fprintf(g_mp4_log, "modification_time:    %lld (%s) 文件创建时间 时间戳从1904.1.1开始计算\n", modification_time, stamp_tmep);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "错误:timescale不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		timescale = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "timescale        :    %d  将1秒钟分为%d份\n", timescale, timescale);
		mp4Info.trackInfor[mp4Info.current_track_ID].timescale = timescale;

		ret = fread(data, 1, 8, g_mp4_file);
		if (ret !=8)
		{
			fprintf(g_mp4_log, "错误:duration不够8字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		duration = (((unsigned __int64)data[0]) << 56) + (((unsigned __int64)data[1]) << 48) + (((unsigned __int64)data[2]) << 40) + (((unsigned __int64)data[3]) << 32) + (((unsigned __int64)data[4]) << 24) + (((unsigned __int64)data[5]) << 16) + (((unsigned __int64)data[6]) << 8) + (((unsigned __int64)data[7]) << 0);
		fprintf(g_mp4_log, "duration         :    %lld 视频播放时长%lld秒\n", duration, duration / timescale);

		read_bytes += 28;
	}
	else
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "错误:creation_time不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		creation_time = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
		char stamp_tmep[128];
		printfTimeStampFROM19040101(creation_time, stamp_tmep);
		fprintf(g_mp4_log, "creation_time    :    %lld (%s) 文件创建时间 时间戳从1904.1.1开始计算\n", creation_time, stamp_tmep);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "错误:modification_time不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		modification_time = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
		printfTimeStampFROM19040101(modification_time, stamp_tmep);
		fprintf(g_mp4_log, "modification_time:    %lld (%s) 文件修改时间 时间戳从1904.1.1开始计算\n", modification_time, stamp_tmep);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "错误:timescale不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		timescale = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "timescale        :    %d  将1秒钟分为%d份\n", timescale, timescale);
		mp4Info.trackInfor[mp4Info.current_track_ID].timescale = timescale;
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "错误:timescale不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		duration = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "duration         :    %lld 视频播放时长%lld秒\n", duration, duration / timescale);

		read_bytes += 16;
	}

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "错误:language不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	language[0] = ((data[0] & 0x7F) >> 2) + 0x60;
	language[1] = (((data[0] & 0x03) << 3) + ((data[1] & 0xE0) >> 5)) + 0x60;
	language[2] =  (data[1] & 0x1F) + 0x60;
	fprintf(g_mp4_log, "language         :    %c%c%c\n", language[0], language[1], language[2]);
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "错误:pre_defined不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	pre_defined = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "pre_defined      :    %d 预先定义字段\n", pre_defined);
	if (pre_defined != 0)
	{
		fprintf(g_mp4_log, "错误:pre_defined 应该为0 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	return 0;
}