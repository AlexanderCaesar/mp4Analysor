//作者  蔡砚刚  20180106  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "meta.h"
#include "hdlr.h"
METABOX::METABOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

	subsubboxConter = 0;
}
int METABOX::anlysis()
{
	subsubboxConter = 0;   //初始化为0
	HDLRBOX hdlrbox(boxSize, read_bytes);
	int ret = hdlrbox.anlysis();
	read_bytes = hdlrbox.read_bytes;
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n+++----------meta subbox %5d-----------+++\n", subsubboxConter);
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
			fprintf(g_mp4_log, "错误:METABOX的子box长度(%d)和与MOOVbox长度(%d)不符合error %d\n", read_bytes, boxSize, g_errors); g_errors++;
			break;
		}
	}
	//METABOX概要信息
	fprintf(g_mp4_log, "\nmeta sub box 个数:    %d\n", subsubboxConter);
	return 0;
}