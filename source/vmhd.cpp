//����  �����  20171227  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "vmhd.h"
#include "common.h"
#include <string.h>

#pragma warning(disable:4996)

VMHDBOX::VMHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}
int VMHDBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++VMHDBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
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
		fprintf(g_mp4_log, "����:graphicsmode����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	graphicsmode = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "graphicsmode     :    %d 0��ʾcopy ���������δ����\n", graphicsmode);
	if (graphicsmode != 0)
	{
		fprintf(g_mp4_log, "����:graphicsmode ��Ϊ0 δ֪��� ����� error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:red����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	red = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "red              :    %d \n", red);
	if (red != 0)
	{
		fprintf(g_mp4_log, "����:red ��Ϊ0 δ֪��� ����� error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:green����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	green = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "green            :    %d \n", green);
	if (green != 0)
	{
		fprintf(g_mp4_log, "����:green ��Ϊ0 δ֪��� ����� error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:blue����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	blue = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "blue             :    %d 0��ʾcopy ���������δ����\n", blue);
	if (blue != 0)
	{
		fprintf(g_mp4_log, "����:blue ��Ϊ0 δ֪��� ����� error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;
	return 0;
}