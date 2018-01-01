//����  �����  20171229  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "smhd.h"

#pragma warning(disable:4996)

SMHDBOX::SMHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}
int SMHDBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++SMHDBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
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

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:balance����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	balance = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "balance          :    %d.%d \n", data[0], data[1]);
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:reserved����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "reserved         :    %d  �����ֶ�\n", reserved);
	if (reserved != 0)
	{
		fprintf(g_mp4_log, "����:reserved Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	return 0;
}