//作者  蔡砚刚  20180106  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "stsz.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

#pragma warning(disable:4996)

STSZBOX::STSZBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

}
int STSZBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++STSZBOX 概要信息+++++++++++++++++++++\n");
	unsigned char data[128];
	int ret = fread(&version, 1, 1, g_mp4_file);

	if (ret != 1)
	{
		fprintf(g_mp4_log, "错误:BOX version不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "version            :    %d\n", version);

	ret = fread(data, 1, 3, g_mp4_file);
	if (ret != 3)
	{
		fprintf(g_mp4_log, "错误:flag不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	flag = (((unsigned int)data[0]) << 16) + (((unsigned int)data[1]) << 8) + (((unsigned int)data[2]) << 0);

	fprintf(g_mp4_log, "flag               :    %u\n", flag);

	read_bytes += 4;

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret != 4)
	{
		fprintf(g_mp4_log, "错误:sample_size不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	sample_size = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "sample_size        :    %d \n", sample_size);
	read_bytes += 4;

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret != 4)
	{
		fprintf(g_mp4_log, "错误:sample_count不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	sample_count = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "sample_count       :    %d 帧数\n", sample_count);
	read_bytes += 4;
	if (sample_size != 0 && sample_count != 0)
	{
		fprintf(g_mp4_log, "错误:sample_size %d 不为0 sample_count 应该为 0 error %d\n", sample_size, sample_count, g_errors); g_errors++;
		return -1;
	}
	//if (sample_count != mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count)
	//{
	//	fprintf(g_mp4_log, "错误:sample_count %d 不等于 轨道先前计算的 sample_count %d error %d\n", sample_size, mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count, g_errors); g_errors++;
	//	return -1;
	//}

	fprintf(g_mp4_log, "entry num              size\n");
	if (sample_size != 0)
	{
		sample_count = mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count;
	}
	if (mp4Info.trackInfor[mp4Info.current_track_ID].stsz == NULL)
	{
		mp4Info.trackInfor[mp4Info.current_track_ID].stsz = (MP4STSZ*)malloc(mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count*sizeof(MP4STSZ));
		if (!mp4Info.trackInfor[mp4Info.current_track_ID].stsz)
		{
			fprintf(g_mp4_log, "错误:stsz 申请内存出错 error %d\n", g_errors); g_errors++;
			return -1;
		}
	}
	for (unsigned int entry = 0; entry < sample_count; entry++)
	{
		if (sample_size == 0)
		{
			ret = fread(data, 1, 4, g_mp4_file);
			if (ret != 4)
			{
				fprintf(g_mp4_log, "错误:entry_size不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
				return -1;
			}
			entry_size = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
			read_bytes += 4;
		}
		else
		{
			entry_size = sample_size;
		}
		mp4Info.trackInfor[mp4Info.current_track_ID].stsz[entry].entry_size = entry_size;
	
		fprintf(g_mp4_log, "    %5d             %5d\n", entry, entry_size);
	}


	return 0;
}