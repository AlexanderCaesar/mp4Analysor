//作者  蔡砚刚  20171227  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "vmhd.h"
#include "common.h"
#include <string.h>

#pragma warning(disable:4996)

VMHDBOX::VMHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

}
int VMHDBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++VMHDBOX 概要信息+++++++++++++++++++++\n");
	unsigned char data[128];
	int ret = fread(&version, 1, 1, g_mp4_file);

	if (ret !=1)
	{
		fprintf(g_mp4_log, "错误:BOX version不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "version          :    %d\n", version);

	ret = fread(data, 1, 3, g_mp4_file);
	if (ret !=3)
	{
		fprintf(g_mp4_log, "错误:flag不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	flag = (((unsigned int)data[0]) << 16) + (((unsigned int)data[1]) << 8) + (((unsigned int)data[2]) << 0);
	fprintf(g_mp4_log, "flag             :    %u\n", flag);
	
	read_bytes += 4;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "错误:graphicsmode不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	graphicsmode = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "graphicsmode     :    %d 0表示copy 其它情况暂未遇见\n", graphicsmode);
	if (graphicsmode != 0)
	{
		fprintf(g_mp4_log, "错误:graphicsmode 不为0 未知情况 请详查 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "错误:red不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	red = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "red              :    %d \n", red);
	if (red != 0)
	{
		fprintf(g_mp4_log, "错误:red 不为0 未知情况 请详查 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "错误:green不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	green = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "green            :    %d \n", green);
	if (green != 0)
	{
		fprintf(g_mp4_log, "错误:green 不为0 未知情况 请详查 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "错误:blue不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	blue = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "blue             :    %d 0表示copy 其它情况暂未遇见\n", blue);
	if (blue != 0)
	{
		fprintf(g_mp4_log, "错误:blue 不为0 未知情况 请详查 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;
	return 0;
}