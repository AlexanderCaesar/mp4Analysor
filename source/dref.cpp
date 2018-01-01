//����  �����  20171229  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "dref.h"
DREFBOX::DREFBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

	subsubboxConter = 0;
}
int DREFBOX::anlysis()
{
	subsubboxConter = 0;   //��ʼ��Ϊ0
	fprintf(g_mp4_log, "\n++++++++++++++++++DREFBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
	unsigned char data[128];
	int ret = fread(&version, 1, 1, g_mp4_file);

	if (ret != 1)
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

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:entry_count����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	entry_count = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	read_bytes += 4;
	fprintf(g_mp4_log, "entry_count      :    %u ���������\n", entry_count);
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n++++++-------dref subbox %5d--------++++++\n", subsubboxConter);
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
			fprintf(g_mp4_log, "����:DREFBOX����box����(%d)����DREFBOX����(%d)������error %d\n", read_bytes, boxSize, g_errors); g_errors++;
			break;
		}
	}
	//DREFBOX��Ҫ��Ϣ
	fprintf(g_mp4_log, "\ndref sub box ����:    %d\n", subsubboxConter);
	return 0;
}