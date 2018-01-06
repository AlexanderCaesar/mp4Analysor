//����  �����  20180106  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "meta.h"
#include "hdlr.h"
METABOX::METABOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

	subsubboxConter = 0;
}
int METABOX::anlysis()
{
	subsubboxConter = 0;   //��ʼ��Ϊ0
	HDLRBOX hdlrbox(boxSize, read_bytes);
	int ret = hdlrbox.anlysis();
	read_bytes = hdlrbox.read_bytes;
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n+++----------meta subbox %5d-----------+++\n", subsubboxConter);
		if (box.readHeader() < 0)
			break;

		if (box.anlysis() < 0)//����box
			break;

		read_bytes += box.boxSize;

		subsubboxConter++;   //����Tag����    

		if (read_bytes == boxSize)
		{
			break;
		}
		else if (read_bytes > boxSize)
		{
			fprintf(g_mp4_log, "����:METABOX����box����(%d)����MOOVbox����(%d)������error %d\n", read_bytes, boxSize, g_errors); g_errors++;
			break;
		}
	}
	//METABOX��Ҫ��Ϣ
	fprintf(g_mp4_log, "\nmeta sub box ����:    %d\n", subsubboxConter);
	return 0;
}