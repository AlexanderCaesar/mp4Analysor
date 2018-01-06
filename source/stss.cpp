//作者  蔡砚刚  20180105  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "stss.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

#pragma warning(disable:4996)

STSSBOX::STSSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

}
int STSSBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++STSSBOX 概要信息+++++++++++++++++++++\n");
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

	mp4Info.trackInfor[mp4Info.current_track_ID].stss_entry_count = entry_count;

	mp4Info.trackInfor[mp4Info.current_track_ID].stss = (MP4STSS*)malloc(entry_count*sizeof(MP4STSS));
	if (!mp4Info.trackInfor[mp4Info.current_track_ID].stss)
	{
		fprintf(g_mp4_log, "错误:stss 申请内存出错 error %d\n", g_errors); g_errors++;
		return -1;
	}

	fprintf(g_mp4_log, "entry num     关键帧位置（从1开始计数）\n");

	for (unsigned int entry = 0; entry < entry_count; entry++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "错误:sample_number不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		sample_number = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);


		read_bytes += 4;
		fprintf(g_mp4_log, "    %5d             %5d\n", entry, sample_number);
		mp4Info.trackInfor[mp4Info.current_track_ID].stss[entry].sample_number = sample_number;
	}


	return 0;
}