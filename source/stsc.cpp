//����  �����  20180105  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "stsc.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

#pragma warning(disable:4996)

STSCBOX::STSCBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}
int STSCBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++STSCBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
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

	mp4Info.trackInfor[mp4Info.current_track_ID].stsc_entry_count = entry_count;

	mp4Info.trackInfor[mp4Info.current_track_ID].stsc = (MP4STSC*)malloc(entry_count*sizeof(MP4STSC));
	if (!mp4Info.trackInfor[mp4Info.current_track_ID].stsc)
	{
		fprintf(g_mp4_log, "����:stsc �����ڴ���� error %d\n", g_errors); g_errors++;
		return -1;
	}

	fprintf(g_mp4_log, "entry num     first_chunk     samples_per_chunk    sample_description_index\n");

	for (unsigned int entry = 0; entry < entry_count; entry++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "����:first_chunk����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		first_chunk = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
		read_bytes += 4;

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "����:samples_per_chunk����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		samples_per_chunk = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
		read_bytes += 4;

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "����:sample_description_index����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		sample_description_index = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
		read_bytes += 4;
		fprintf(g_mp4_log, "    %5d           %5d                 %5d                       %5d\n", entry, first_chunk, samples_per_chunk,sample_description_index);
		mp4Info.trackInfor[mp4Info.current_track_ID].stsc[entry].first_chunk = first_chunk;
		mp4Info.trackInfor[mp4Info.current_track_ID].stsc[entry].samples_per_chunk = samples_per_chunk;
		mp4Info.trackInfor[mp4Info.current_track_ID].stsc[entry].sample_description_index = sample_description_index;
	}

	return 0;
}