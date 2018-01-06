//����  �����  20171227  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "elst.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

#pragma warning(disable:4996)

ELSTBOX::ELSTBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}
int ELSTBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++ELSTBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
	unsigned char data[128];
	int ret = fread(&version, 1, 1, g_mp4_file);

	if (ret !=1)
	{
		fprintf(g_mp4_log, "����:BOX version����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "version            :    %d\n", version);

	ret = fread(data, 1, 3, g_mp4_file);
	if (ret !=3)
	{
		fprintf(g_mp4_log, "����:flag����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	flag = (((unsigned int)data[0]) << 16) + (((unsigned int)data[1]) << 8) + (((unsigned int)data[2]) << 0);
	
	fprintf(g_mp4_log, "flag               :    %u\n", flag);
	
	read_bytes += 4;

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:entry_count����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	entry_count = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "entry_count        :    %d ý��Ƭ�θ���\n", entry_count);
	read_bytes += 4;

	mp4Info.trackInfor[mp4Info.current_track_ID].elst_entry_count = entry_count;

	mp4Info.trackInfor[mp4Info.current_track_ID].elst = (MP4ELST*)malloc(entry_count*sizeof(MP4ELST));
	if (!mp4Info.trackInfor[mp4Info.current_track_ID].elst)
	{
		fprintf(g_mp4_log, "����:elst �����ڴ���� error %d\n", g_errors); g_errors++;
		return -1;
	}
	for (unsigned int entry = 0; entry < entry_count; entry++)
	{
		fprintf(g_mp4_log, "entry num          :    %d\n", entry);

		if (version == 1)
		{
			ret = fread(data, 1, 8, g_mp4_file);
			if (ret !=8)
			{
				fprintf(g_mp4_log, "����:segment_duration����8�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
				return -1;
			}
			segment_duration = (((unsigned __int64)data[0]) << 56) + (((unsigned __int64)data[1]) << 48) + (((unsigned __int64)data[2]) << 40) + (((unsigned __int64)data[3]) << 32) + (((unsigned __int64)data[4]) << 24) + (((unsigned __int64)data[5]) << 16) + (((unsigned __int64)data[6]) << 8) + (((unsigned __int64)data[7]) << 0);
			fprintf(g_mp4_log, "segment_duration   :    %lld ʱ��(%lld)�� \n", segment_duration, segment_duration / mp4Info.timescale);

			ret = fread(data, 1, 8, g_mp4_file);
			if (ret !=8)
			{
				fprintf(g_mp4_log, "����:media_time����8�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
				return -1;
			}
			media_time = (__int64)((((unsigned __int64)data[0]) << 56) + (((unsigned __int64)data[1]) << 48) + (((unsigned __int64)data[2]) << 40) + (((unsigned __int64)data[3]) << 32) + (((unsigned __int64)data[4]) << 24) + (((unsigned __int64)data[5]) << 16) + (((unsigned __int64)data[6]) << 8) + (((unsigned __int64)data[7]) << 0));
			char rtime[128];
			printfTimeStamp((unsigned __int64)media_time, rtime);
			fprintf(g_mp4_log, "media_time         :    %lld  %s ý��Ƭ�β�����ʼʱ�� ���Ϊ-1 ˵���ǿ�Ƭ�� \n", media_time, rtime);
			read_bytes += 16;
		}
		else
		{
			ret = fread(data, 1, 4, g_mp4_file);
			if (ret !=4)
			{
				fprintf(g_mp4_log, "����:segment_duration����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
				return -1;
			}
			segment_duration = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
			fprintf(g_mp4_log, "segment_duration   :    %lld ʱ��(%lld)�� \n", segment_duration, segment_duration / mp4Info.timescale);

			ret = fread(data, 1, 4, g_mp4_file);
			if (ret !=4)
			{
				fprintf(g_mp4_log, "����:media_time����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
				return -1;
			}
			media_time = ((((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0));
			char rtime[128];
			printfTimeStamp((unsigned __int64)media_time , rtime);
			fprintf(g_mp4_log, "media_time         :    %lld  %s ý��Ƭ�β�����ʼʱ�� ���Ϊ-1 ˵���ǿ�Ƭ�� \n", media_time, rtime);
			read_bytes += 8;

			mp4Info.trackInfor[mp4Info.current_track_ID].elst[entry].media_time = media_time;
			mp4Info.trackInfor[mp4Info.current_track_ID].elst[entry].segment_duration = segment_duration;
		}

		ret = fread(data, 1, 2, g_mp4_file);
		if (ret !=2)
		{
			fprintf(g_mp4_log, "����:media_rate_integer����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		media_rate_integer = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
		fprintf(g_mp4_log, "media_rate_integer :    %d ��������0��ʾ��ǰΪ��ֹ����\n", media_rate_integer);
		read_bytes += 2;

		ret = fread(data, 1, 2, g_mp4_file);
		if (ret !=2)
		{
			fprintf(g_mp4_log, "����:media_rate_fraction����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		media_rate_fraction = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
		fprintf(g_mp4_log, "media_rate_fraction:    %d ��������0��ʾ��ǰΪ��ֹ����\n", media_rate_fraction);
		read_bytes += 2;
	}

	return 0;
}