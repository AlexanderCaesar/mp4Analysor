//作者  蔡砚刚  20171229  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "stbl.h"
STBLBOX::STBLBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

	subsubboxConter = 0;
}
int STBLBOX::anlysis()
{
	subsubboxConter = 0;   //初始化为0
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n+++++--------stbl subbox %5d---------+++++\n", subsubboxConter);
		if (box.readHeader() < 0)
			break;

		if (box.anlysis() < 0)//分析box
			break;

		read_bytes += box.boxSize;

		subsubboxConter++;   //计数Tag个数    

		if (read_bytes == boxSize)
		{
			break;
		}
		else if (read_bytes > boxSize)
		{
			fprintf(g_mp4_log, "错误:STBLBOX的子box长度(%d)和与STBLBOX长度(%d)不符合error %d\n", read_bytes, boxSize, g_errors); g_errors++;
			break;
		}
	}
	//STBLBOX概要信息
	fprintf(g_mp4_log, "\nstbl sub box 个数:    %d\n", subsubboxConter);
	return 0;
}