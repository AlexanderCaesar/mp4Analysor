//����  �����  20171227  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "minf.h"
MINFBOX::MINFBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

	subsubboxConter = 0;
}
int MINFBOX::anlysis()
{
	subsubboxConter = 0;   //��ʼ��Ϊ0
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n++++---------minf subbox %5d----------++++\n", subsubboxConter);
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
			fprintf(g_mp4_log, "����:MDIABOX����box����(%d)����MOOVbox����(%d)������error %d\n", read_bytes, boxSize, g_errors); g_errors++;
			break;
		}
	}
	//MINFBOX��Ҫ��Ϣ
	fprintf(g_mp4_log, "\nminf sub box ����:    %d\n", subsubboxConter);
	return 0;
}