//作者  蔡砚刚  20171226  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "moov.h"
int MOOVBOX::count = 0;
MOOVBOX::MOOVBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

	subboxConter = 0;
}
int MOOVBOX::anlysis()
{
	if (count > 0)
	{
		fprintf(g_mp4_log, "错误:FTYPBOX出现次数多余一次 count：%d error %d\n", count, g_errors); g_errors++;
	}
	count++;

	subboxConter = 0;   //初始化为0
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n+-----------------moov subbox %5d--------------------+\n", subboxConter);
		if (box.readHeader() < 0)
			break;

		if (box.anlysis() < 0)//分析box
			break;

		read_bytes += box.boxSize;

		subboxConter++;   //计数Tag个数    

		if (read_bytes == boxSize)
		{
			break;
		}
		else if (read_bytes > boxSize)
		{
			fprintf(g_mp4_log, "错误:MOOV的子box长度(%d)和与MOOVbox长度(%d)不符合error %d\n", read_bytes, boxSize, g_errors); g_errors++;
			break;
		}
	}
	//moov概要信息
	fprintf(g_mp4_log, "\nmoov sub box 个数:    %d\n", subboxConter);
	return 0;
}