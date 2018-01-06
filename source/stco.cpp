//����  �����  20180106  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "stco.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

#pragma warning(disable:4996)

STCOBOX::STCOBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}
int STCOBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++STCOBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
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
	fprintf(g_mp4_log, "entry_count        :    %d \n", entry_count);
	read_bytes += 4;


	fprintf(g_mp4_log, "entry num         chunk_offset     samples_per_chunk\n");
	if (mp4Info.trackInfor[mp4Info.current_track_ID].stco == NULL)
	{
		mp4Info.trackInfor[mp4Info.current_track_ID].stco = (MP4STCO*)malloc(mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count*sizeof(MP4STCO));
		if (!mp4Info.trackInfor[mp4Info.current_track_ID].stco)
		{
			fprintf(g_mp4_log, "����:stco �����ڴ���� error %d\n", g_errors); g_errors++;
			return -1;
		}
	}
	for (unsigned int i = 0; i < mp4Info.trackInfor[mp4Info.current_track_ID].stsc_entry_count-1; i++)
	{
		for (unsigned int j = mp4Info.trackInfor[mp4Info.current_track_ID].stsc[i].first_chunk - 1; j < mp4Info.trackInfor[mp4Info.current_track_ID].stsc[i + 1].first_chunk; j++)
		{
			mp4Info.trackInfor[mp4Info.current_track_ID].stco[j].samples_per_chunk = mp4Info.trackInfor[mp4Info.current_track_ID].stsc[i].samples_per_chunk;
		}
	}
	for (unsigned int j = mp4Info.trackInfor[mp4Info.current_track_ID].stsc[mp4Info.trackInfor[mp4Info.current_track_ID].stsc_entry_count - 1].first_chunk - 1; j < entry_count; j++)
	{
		mp4Info.trackInfor[mp4Info.current_track_ID].stco[j].samples_per_chunk = mp4Info.trackInfor[mp4Info.current_track_ID].stsc[mp4Info.trackInfor[mp4Info.current_track_ID].stsc_entry_count - 1].samples_per_chunk;
	}
	for (unsigned int entry = 0; entry < entry_count; entry++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "����:entry_size����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		chunk_offset = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
		read_bytes += 4;
		mp4Info.trackInfor[mp4Info.current_track_ID].stco[entry].chunk_offset = chunk_offset;
	
		fprintf(g_mp4_log, "    %5d             %8d       %5d\n", entry, chunk_offset, mp4Info.trackInfor[mp4Info.current_track_ID].stco[entry].samples_per_chunk);
	}

	return 0;
}