//����  �����  20171227  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "trhd.h"
#include "common.h"
#include <string.h>

#pragma warning(disable:4996)

TRHDBOX::TRHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}
int TRHDBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++TRHDBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
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
	char str_temp[128];
	switch (flag)
	{
	case 0:strcpy(str_temp, "Track_disabled"); break;
	case 1:strcpy(str_temp, "Track_enabled"); break;
	case 2:strcpy(str_temp, "Track_in_movie"); break;
	case 3:strcpy(str_temp, "Track_enabled Track_in_movie"); break;
	case 4:strcpy(str_temp, "Track_in_preview"); break;
	default:strcpy(str_temp, "Track_ERROR"); fprintf(g_mp4_log, "����:flag���ݴ��� �ļ����� flag %d error %d\n", flag, g_errors); g_errors++;
	}
	fprintf(g_mp4_log, "flag             :    %u %s\n", flag,str_temp);
	
	read_bytes += 4;
	if (version == 1)
	{
		ret = fread(data, 1, 8, g_mp4_file);
		if (ret !=8)
		{
			fprintf(g_mp4_log, "����:creation_time����8�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		creation_time = (((unsigned __int64)data[0]) << 56) + (((unsigned __int64)data[1]) << 48) + (((unsigned __int64)data[2]) << 40) + (((unsigned __int64)data[3]) << 32) + (((unsigned __int64)data[4]) << 24) + (((unsigned __int64)data[5]) << 16) + (((unsigned __int64)data[6]) << 8) + (((unsigned __int64)data[7]) << 0);
		char stamp_tmep[128];
		printfTimeStampFROM19040101(creation_time, stamp_tmep);
		fprintf(g_mp4_log, "creation_time    :    %lld (%s) �ļ�����ʱ�� ʱ�����1904.1.1��ʼ����\n", creation_time, stamp_tmep);

		ret = fread(data, 1, 8, g_mp4_file);
		if (ret !=8)
		{
			fprintf(g_mp4_log, "����:modification_time����8�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		modification_time = (((unsigned __int64)data[0]) << 56) + (((unsigned __int64)data[1]) << 48) + (((unsigned __int64)data[2]) << 40) + (((unsigned __int64)data[3]) << 32) + (((unsigned __int64)data[4]) << 24) + (((unsigned __int64)data[5]) << 16) + (((unsigned __int64)data[6]) << 8) + (((unsigned __int64)data[7]) << 0);
		printfTimeStampFROM19040101(modification_time, stamp_tmep);
		fprintf(g_mp4_log, "modification_time:    %lld (%s) �ļ�����ʱ�� ʱ�����1904.1.1��ʼ����\n", modification_time, stamp_tmep);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:track_ID����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		track_ID = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "track_ID         :    %d  \n", track_ID);

		mp4Info.current_track_ID = track_ID;
		if (track_ID > MAX_TRACKINFOR_NUM)
		{
			fprintf(g_mp4_log, "����:track_ID̫�� ���ܳ���  MAX_TRACKINFOR_NUM %d error %d\n", MAX_TRACKINFOR_NUM,g_errors); g_errors++;
			return -1;
		}

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:reserved0����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		reserved0 = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "reserved0        :    %d  \n", reserved0);

		ret = fread(data, 1, 8, g_mp4_file);
		if (ret !=8)
		{
			fprintf(g_mp4_log, "����:duration����8�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		duration = (((unsigned __int64)data[0]) << 56) + (((unsigned __int64)data[1]) << 48) + (((unsigned __int64)data[2]) << 40) + (((unsigned __int64)data[3]) << 32) + (((unsigned __int64)data[4]) << 24) + (((unsigned __int64)data[5]) << 16) + (((unsigned __int64)data[6]) << 8) + (((unsigned __int64)data[7]) << 0);
		fprintf(g_mp4_log, "duration         :    %lld ��Ƶ����ʱ��%lld��\n", duration, duration / mp4Info.timescale);

		read_bytes += 32;
	}
	else
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:creation_time����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		creation_time = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
		char stamp_tmep[128];
		printfTimeStampFROM19040101(creation_time, stamp_tmep);
		fprintf(g_mp4_log, "creation_time    :    %lld (%s) �ļ�����ʱ�� ʱ�����1904.1.1��ʼ����\n", creation_time, stamp_tmep);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:modification_time����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		modification_time = (((unsigned __int64)data[0]) << 24) + (((unsigned __int64)data[1]) << 16) + (((unsigned __int64)data[2]) << 8) + (((unsigned __int64)data[3]) << 0);
		printfTimeStampFROM19040101(modification_time, stamp_tmep);
		fprintf(g_mp4_log, "modification_time:    %lld (%s) �ļ��޸�ʱ�� ʱ�����1904.1.1��ʼ����\n", modification_time, stamp_tmep);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:track_ID����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		track_ID = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "track_ID         :    %d  \n", track_ID);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:reserved0����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		reserved0 = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "reserved0        :    %d  \n", reserved0);

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:timescale����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		duration = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "duration         :    %lld ��Ƶ����ʱ��%lld��\n", duration, duration / mp4Info.timescale);

		read_bytes += 20;
	}

	for (int i = 0; i < 2; i++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:reserved2����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		reserved2[i] = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "reserved2[%d]     :    %d �����ֶ�\n", i, reserved2[i]);
		if (reserved2[i] != 0)
		{
			fprintf(g_mp4_log, "����:reserved2 Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
		}
		read_bytes += 4;
	}

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:layer����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	layer = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "layer            :    %d ���Ų���� ���ԽСԽ��������\n", layer);
	read_bytes += 2;


	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:alternate_group����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	alternate_group = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "alternate_group  :    %d track���\n", alternate_group);
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:volume����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	volume = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "volume           :    %d.%d ������������ 1.0��ʾ������\n", data[0], data[1]);
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:reserved1����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved1 = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "reserved1        :    %d  �����ֶ�\n", reserved1);
	if (reserved1 != 0)
	{
		fprintf(g_mp4_log, "����:reserved1 Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	for (int i = 0; i < 9; i++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:matrix����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		matrix[i] = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		read_bytes += 4;
	}
	fprintf(g_mp4_log, "matrix           :    ��Ƶת������ ����ϸ�ڴ���ѯ { 0x00010000,0x0,0x0,0x0,0x00010000,0x0,0x0,0x0,0x40000000 };\n");
	for (int i = 0; i < 3; i++)
	{
		fprintf(g_mp4_log, "                   ");
		for (int j = 0; j < 3; j++)
		{
			fprintf(g_mp4_log, "%12d",matrix[i*3+j]);
		}
		fprintf(g_mp4_log, "\n");
	}


	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:width����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	width = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	int x1 = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	int x2 = (((unsigned int)data[2]) << 9) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "width            :    %d.%d ��Ƶ���\n", x1,x2);
	read_bytes += 4;

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:height����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	height = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	x1 = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	x2 = (((unsigned int)data[2]) << 9) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "height           :    %d.%d ��Ƶ�߶�\n", x1, x2);
	read_bytes += 4;

	return 0;
}