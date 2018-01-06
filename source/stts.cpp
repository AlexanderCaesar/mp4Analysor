//����  �����  20180103  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "stts.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

#pragma warning(disable:4996)

STTSBOX::STTSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}
int STTSBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++STTSBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
	unsigned char data[128];
	int ret = fread(&version, 1, 1, g_mp4_file);

	if (ret != 1)
	{
		fprintf(g_mp4_log, "����:BOX version����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "version            :    %d\n", version);

	ret = fread(data, 1, 3, g_mp4_file);
	if (ret != 3)
	{
		fprintf(g_mp4_log, "����:flag����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	flag = (((unsigned int)data[0]) << 16) + (((unsigned int)data[1]) << 8) + (((unsigned int)data[2]) << 0);

	fprintf(g_mp4_log, "flag               :    %u\n", flag);

	read_bytes += 4;

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret != 4)
	{
		fprintf(g_mp4_log, "����:entry_count����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	entry_count = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "entry_count        :    %d ý��Ƭ�θ���\n", entry_count);
	read_bytes += 4;

	mp4Info.trackInfor[mp4Info.current_track_ID].stts_entry_count = entry_count;

	mp4Info.trackInfor[mp4Info.current_track_ID].stts = (MP4STTS*)malloc(entry_count*sizeof(MP4STTS));
	if (!mp4Info.trackInfor[mp4Info.current_track_ID].stts)
	{
		fprintf(g_mp4_log, "����:stts �����ڴ���� error %d\n", g_errors); g_errors++;
		return -1;
	}

	fprintf(g_mp4_log, "entry num     entry_countý��Ƭ�θ���     sample_deltaÿ��Ƭ�εĲ���ʱ��\n");

	double sum_duration = 0.0;
	int    sum_sample_count = 0;
	for (unsigned int entry = 0; entry < entry_count; entry++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "����:sample_count����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		sample_count = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "����:sample_delta����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		sample_delta = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
		sum_duration += (double)(sample_count*sample_delta);
		sum_sample_count += sample_count;
		read_bytes += 8;
		fprintf(g_mp4_log, "    %5d             %5d                 %5d��%.5lf�룩\n", entry, sample_count, sample_delta, (double)sample_delta / (double)mp4Info.trackInfor[mp4Info.current_track_ID].timescale);
		mp4Info.trackInfor[mp4Info.current_track_ID].stts[entry].sample_count = sample_count;
		mp4Info.trackInfor[mp4Info.current_track_ID].stts[entry].sample_delta = sample_delta;
	}
	fprintf(g_mp4_log, "sum_duration       :    %0.5lf ��\n", sum_duration/(double)mp4Info.trackInfor[mp4Info.current_track_ID].timescale);
	fprintf(g_mp4_log, "sum_sample_count   :    %d NAL��Ԫ����֡��\n", sum_sample_count);
	if (mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count == 0)
	{
		mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count = sum_sample_count;
	}
	if (sum_sample_count != mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count)
	{
		fprintf(g_mp4_log, "����:sum_sample_count %d ��ǰ�������sum_sample_count %d ��һ�� error %d\n", sum_sample_count, mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count, g_errors); g_errors++;
	}
	return 0;
}