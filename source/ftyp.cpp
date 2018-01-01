//����  �����  20171226  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "ftyp.h"
int FTYPBOX::count = 0;
FTYPBOX::FTYPBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	major_brand = 0;  // 32λ is a brand identifier
	minor_version=0;// 32λ s an informative integer for the minor version of the major brand

	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ
}
int FTYPBOX::anlysis()
{
	if (count > 0)
	{
		fprintf(g_mp4_log, "����:FTYPBOX���ִ�������һ�� count��%d error %d\n", count,g_errors); g_errors++;
	}
	count++;
	unsigned char data[12];
	int ret = fread(data, 1, 4, g_mp4_file);
	fprintf(g_mp4_log, "\n++++++++++++++++++FTYPBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");

    if (ret !=4)
	{
		fprintf(g_mp4_log, "����:major_brand����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	major_brand = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "major_brand:    %c%c%c%c(%d)\n", data[0],data[1],data[2],data[3],major_brand);

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:minor_version����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	minor_version = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "minor_version:  %d\n", minor_version);

	read_bytes += 8;

	long size = (long)(boxSize - read_bytes);
	if (size >= MAX_COMPATIBLE_BRANDS_CHARS)
	{
		ret = fseek(g_mp4_file, size, SEEK_CUR);
		fprintf(g_mp4_log, "����:size %d bigger than MAX_COMPATIBLE_BRANDS_CHARS %d error %d\n", size,MAX_COMPATIBLE_BRANDS_CHARS,g_errors); g_errors++;
		if (ret < 0)
		{
			fprintf(g_mp4_log, "����:�ļ���������error %d\n", g_errors); g_errors++;
		}
		return 0;
	}
	else
	{
		ret = fread(compatible_brands, 1, size, g_mp4_file);
		if (ret !=size)
		{
			fprintf(g_mp4_log, "����:compatible_brands����size(%d)�ֽ� �ļ����� error %d\n",size, g_errors); g_errors++;
			return -1;
		}
		compatible_brands[size] = '\0';
		fprintf(g_mp4_log, "compatible_brands: %s\n", compatible_brands);
		read_bytes += size;
		return 0;
	}
	read_bytes += size;
	return 0;
}