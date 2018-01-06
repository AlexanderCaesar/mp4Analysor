//作者  蔡砚刚  20180106  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "mdat.h"
MDATBOX::MDATBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

	subboxConter = 0;
}
int MDATBOX::anlysis()
{
	unsigned char data[12];
	int ret = 0;


	subboxConter = 0;   //初始化为0
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n+-----------------mdat subbox %5d--------------------+\n", subboxConter);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "错误:reserved不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		unsigned int size = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		ret = fread(data, 1, 2, g_mp4_file);

		ret = fseek(g_mp4_file, (long)(size - 2), SEEK_CUR);

		fprintf(g_mp4_log, "size %d  %d 0x%x  0x%x\n", size,size>>16, data[0],data[1]);
	

		subboxConter++;   //计数Tag个数    


	}
	//mdat概要信息
	fprintf(g_mp4_log, "\nmdat sub box 个数:    %d\n", subboxConter);
	return 0;
}