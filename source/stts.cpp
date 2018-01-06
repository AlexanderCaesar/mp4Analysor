//作者  蔡砚刚  20180103  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "stts.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

#pragma warning(disable:4996)

STTSBOX::STTSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

}
int STTSBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++STTSBOX 概要信息+++++++++++++++++++++\n");
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
		fprintf(g_mp4_log, "错误:entry_count不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	entry_count = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "entry_count        :    %d 媒体片段个数\n", entry_count);
	read_bytes += 4;

	mp4Info.trackInfor[mp4Info.current_track_ID].stts_entry_count = entry_count;

	mp4Info.trackInfor[mp4Info.current_track_ID].stts = (MP4STTS*)malloc(entry_count*sizeof(MP4STTS));
	if (!mp4Info.trackInfor[mp4Info.current_track_ID].stts)
	{
		fprintf(g_mp4_log, "错误:stts 申请内存出错 error %d\n", g_errors); g_errors++;
		return -1;
	}

	fprintf(g_mp4_log, "entry num     entry_count媒体片段个数     sample_delta每个片段的播放时间\n");

	double sum_duration = 0.0;
	int    sum_sample_count = 0;
	for (unsigned int entry = 0; entry < entry_count; entry++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "错误:sample_count不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		sample_count = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "错误:sample_delta不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		sample_delta = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
		sum_duration += (double)(sample_count*sample_delta);
		sum_sample_count += sample_count;
		read_bytes += 8;
		fprintf(g_mp4_log, "    %5d             %5d                 %5d（%.5lf秒）\n", entry, sample_count, sample_delta, (double)sample_delta / (double)mp4Info.trackInfor[mp4Info.current_track_ID].timescale);
		mp4Info.trackInfor[mp4Info.current_track_ID].stts[entry].sample_count = sample_count;
		mp4Info.trackInfor[mp4Info.current_track_ID].stts[entry].sample_delta = sample_delta;
	}
	fprintf(g_mp4_log, "sum_duration       :    %0.5lf 秒\n", sum_duration/(double)mp4Info.trackInfor[mp4Info.current_track_ID].timescale);
	fprintf(g_mp4_log, "sum_sample_count   :    %d NAL单元或者帧数\n", sum_sample_count);
	if (mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count == 0)
	{
		mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count = sum_sample_count;
	}
	if (sum_sample_count != mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count)
	{
		fprintf(g_mp4_log, "错误:sum_sample_count %d 与前面分析的sum_sample_count %d 不一致 error %d\n", sum_sample_count, mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count, g_errors); g_errors++;
	}
	return 0;
}