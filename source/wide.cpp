//����  �����  20171229  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "wide.h"

#pragma warning(disable:4996)

OTHERBOX::OTHERBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}
int OTHERBOX::anlysis()
{
	long size = (long)(boxSize - read_bytes);
	int ret = 0;
	fprintf(g_mp4_log, "size             : %d\n", size);

	if (size == 0) return 0;

	if (size >= MAX_WIDE_CHARS || size < 0)
	{
		ret = fseek(g_mp4_file, size, SEEK_CUR);
		fprintf(g_mp4_log, "����:size %d bigger than MAX_WIDE_CHARS %d error %d\n", size, MAX_WIDE_CHARS, g_errors); g_errors++;
		if (ret < 0)
		{
			fprintf(g_mp4_log, "����:�ļ���������error %d\n", g_errors); g_errors++;
		}
		return 0;
	}
	else
	{
		ret = fread(data, 1, size, g_mp4_file);
		if (ret !=size)
		{
			fprintf(g_mp4_log, "����:data����size(%d)�ֽ� �ļ����� error %d\n", size, g_errors); g_errors++;
			return -1;
		}
		data[size] = '\0';
		fprintf(g_mp4_log, "data             : %s\n", data);
		read_bytes += size;
		return 0;
	}
	read_bytes += size;
	return 0;
}