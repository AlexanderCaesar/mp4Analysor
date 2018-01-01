//����  �����  20171227  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "hdlr.h"
#include "common.h"
#include <string.h>

#pragma warning(disable:4996)

HDLRBOX::HDLRBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}
int HDLRBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++HDLRBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
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

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:pre_defined����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	pre_defined = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "pre_defined      :    %d Ԥ�ȶ����ֶ�\n", pre_defined);
	if (pre_defined != 0)
	{
		fprintf(g_mp4_log, "����:pre_defined Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 4;

	ret = fread(handler_type.c_type, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:handler_type����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	mp4Info.trackInfor[mp4Info.current_track_ID].track_ID = mp4Info.current_track_ID;
	mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.i_type = handler_type.i_type;
	switch (handler_type.i_type)
	{
	case HANDLETYPE_VIDE: mp4Info.video_track_ID = mp4Info.current_track_ID; break;
	case HANDLETYPE_SOUN: mp4Info.audio_track_ID= mp4Info.current_track_ID; break;
	default:
		fprintf(g_mp4_log, "����:handler_type ������error %d\n", g_errors); g_errors++;
		break;
	}

	fprintf(g_mp4_log, "handler_type     :    %d %c%c%c%c\n", handler_type.i_type, handler_type.c_type[0], handler_type.c_type[1], handler_type.c_type[2], handler_type.c_type[3]);
	read_bytes += 4;

	for (int i = 0; i < 3; i++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:reserved����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		reserved[i] = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "reserved[%d]      :    %d �����ֶ�\n",i, reserved[i]);
		if (reserved[i] != 0)
		{
			fprintf(g_mp4_log, "����:reserved Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
		}
		read_bytes += 4;
	}
	long size = (long)(boxSize - read_bytes);
	if (size >= MAX_HDLRBOX_NAME_CHARS)
	{
		ret = fseek(g_mp4_file, size, SEEK_CUR);
		fprintf(g_mp4_log, "����:size %d bigger than MAX_HDLRBOX_NAME_CHARS %d error %d\n", size, MAX_HDLRBOX_NAME_CHARS, g_errors); g_errors++;
		if (ret < 0)
		{
			fprintf(g_mp4_log, "����:�ļ���������error %d\n", g_errors); g_errors++;
		}
		return 0;
	}
	else
	{
		ret = fread(name, 1, size, g_mp4_file);
		if (ret !=size)
		{
			fprintf(g_mp4_log, "����:compatible_brands����size(%d)�ֽ� �ļ����� error %d\n", size, g_errors); g_errors++;
			return -1;
		}
		name[size] = '\0';
		fprintf(g_mp4_log, "name             :    %s\n", name);
		read_bytes += size;
		return 0;
	}
	read_bytes += size;

	return 0;
}