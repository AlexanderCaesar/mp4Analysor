//作者  蔡砚刚  20171226  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "mvhd.h"
#include "common.h"
int MVHDBOX::count = 0;
MVHDBOX::MVHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

	subboxConter = 0;
}
int MVHDBOX::anlysis()
{
	if (count > 0)
	{
		fprintf(g_mp4_log, "错误:MVHDBOX出现次数多余一次 count：%d error %d\n", count, g_errors); g_errors++;
	}
	count++;

	fprintf(g_mp4_log, "\n++++++++++++++++++MVHDBOX 概要信息+++++++++++++++++++++\n");
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

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "错误:rate不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	rate = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	int x1 = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	int x2 = (((unsigned int)data[2]) << 9) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "rate             :    %d.%d 播放速率\n", x1,x2);
	read_bytes += 4;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "错误:volume不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	volume = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "volume           :    %d.%d 播放声音音量 1.0表示满音量\n", data[0], data[1]);
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "错误:reserved1不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved1 = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "reserved1        :    %d  保留字段\n", reserved1);
	if (reserved1 != 0)
	{
		fprintf(g_mp4_log, "错误:reserved1 应该为0 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	for (int i = 0; i < 2; i++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "错误:reserved2不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		reserved2[i] = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "reserved2[%d]     :    %d 保留字段\n", i,reserved2[i]);
		if (reserved2[i] != 0)
		{
			fprintf(g_mp4_log, "错误:reserved2 应该为0 error %d\n", g_errors); g_errors++;
		}
		read_bytes += 4;
	}

	for (int i = 0; i < 9; i++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "错误:matrix不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		matrix[i] = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		read_bytes += 4;
	}
	fprintf(g_mp4_log, "matrix           :    视频转换矩阵 具体细节待查询 { 0x00010000,0x0,0x0,0x0,0x00010000,0x0,0x0,0x0,0x40000000 };\n");
	for (int i = 0; i < 3; i++)
	{
		fprintf(g_mp4_log, "                   ");
		for (int j = 0; j < 3; j++)
		{
			fprintf(g_mp4_log, "%12d",matrix[i*3+j]);
		}
		fprintf(g_mp4_log, "\n");
	}

	for (int i = 0; i < 6; i++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "错误:pre_defined不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		pre_defined[i] = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "pre_defined[%d]   :    %d 预先定义字段\n", i, pre_defined[i]);
		if (pre_defined[i] != 0)
		{
			fprintf(g_mp4_log, "错误:pre_defined 应该为0 error %d\n", g_errors); g_errors++;
		}
		read_bytes += 4;
	}

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "错误:next_track_ID不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	next_track_ID = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "next_track_ID    :    %d 下一个next_track_ID号\n", next_track_ID);
	if (next_track_ID == 0)
	{
		fprintf(g_mp4_log, "错误:next_track_ID不应该为0 error %d\n", g_errors); g_errors++;
	}
	mp4Info.timescale = timescale;
	read_bytes += 4;
	return 0;
}