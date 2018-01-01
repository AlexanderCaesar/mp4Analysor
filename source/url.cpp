//����  �����  20171229  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "url.h"

#pragma warning(disable:4996)

URLBOX::URLBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}

int URLBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++URLBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");

	unsigned char data[128];
	int ret = fread(&version, 1, 1, g_mp4_file);

	if (ret !=1)
	{
		fprintf(g_mp4_log, "����:BOX version����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "version          :    %d\n", version);

	ret = fread(data, 1, 3, g_mp4_file);
	if (ret !=3)
	{
		fprintf(g_mp4_log, "����:flag����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	flag = (((unsigned int)data[0]) << 16) + (((unsigned int)data[1]) << 8) + (((unsigned int)data[2]) << 0);
	fprintf(g_mp4_log, "flag             :    %u\n", flag);

	read_bytes += 4;

	fprintf(g_mp4_log, "size             :    %d\n", size);
	long size = (long)(boxSize - read_bytes);

	if (size == 0)
	{
		fprintf(g_mp4_log, "location         :    Same file\n", location);
		return 0;	
	}

	if (size >= MAX_LOCATION_CHARS || size < 0)
	{
		ret = fseek(g_mp4_file, size, SEEK_CUR);
		fprintf(g_mp4_log, "����:size %d bigger than MAX_WIDE_CHARS %d error %d\n", size, MAX_LOCATION_CHARS, g_errors); g_errors++;
		if (ret < 0)
		{
			fprintf(g_mp4_log, "����:�ļ���������error %d\n", g_errors); g_errors++;
		}
		return 0;
	}
	else
	{
		ret = fread(location, 1, size, g_mp4_file);
		if (ret !=size)
		{
			fprintf(g_mp4_log, "����:data����size(%d)�ֽ� �ļ����� error %d\n", size, g_errors); g_errors++;
			return -1;
		}
		location[size] = '\0';
		fprintf(g_mp4_log, "location         : %s\n", location);
		read_bytes += size;
		return 0;
	}
	read_bytes += size;
	return 0;
}