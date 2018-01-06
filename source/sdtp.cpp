//����  �����  20180105  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "sdtp.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>

#pragma warning(disable:4996)

SDTPBOX::SDTPBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

}
int SDTPBOX::anlysis()
{
	fprintf(g_mp4_log, "\n++++++++++++++++++SDTPBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
	unsigned char data[128];
	int ret = fread(&version, 1, 1, g_mp4_file);

	if (ret != 1)
	{
		fprintf(g_mp4_log, "����:BOX version����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "version            :    %d\n", version);

	ret = fread(data, 1, 3, g_mp4_file);
	if (ret != 3)
	{
		fprintf(g_mp4_log, "����:flag����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	flag = (((unsigned int)data[0]) << 16) + (((unsigned int)data[1]) << 8) + (((unsigned int)data[2]) << 0);

	fprintf(g_mp4_log, "flag               :    %u\n", flag);

	read_bytes += 4;

	mp4Info.trackInfor[mp4Info.current_track_ID].sdtp = (MP4SDTP*)malloc(mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count*sizeof(MP4SDTP));
	if (!mp4Info.trackInfor[mp4Info.current_track_ID].sdtp)
	{
		fprintf(g_mp4_log, "����:sdtp �����ڴ���� error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "entry num    �ο�����֡��ϵ      ���ο���ϵ        ��������\n");

	for (unsigned int entry = 0; entry < mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count; entry++)
	{
		ret = fread(data, 1, 1, g_mp4_file);
		if (ret != 1)
		{
			fprintf(g_mp4_log, "����:sample_depends_on����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		reserved = data[0] >> 6;
		sample_depends_on = ((data[0] & 0x30) >> 4);
		sample_is_depended_on = ((data[0] & 0x0C) >> 2);
		sample_has_redundancy = ((data[0] & 0x03) >> 0);
		read_bytes += 1;
		char depends_on[128];
		char is_depended_on[128];
		char has_redundancy[128];
		switch (sample_depends_on)
		{
		case 0: sprintf(depends_on, "%d �ο���֡����", sample_depends_on); break;
		case 1: sprintf(depends_on, "%d �ο���I֡   ", sample_depends_on); break;
		case 2: sprintf(depends_on, "%d �ο�I֡     ", sample_depends_on); break;
		case 3: sprintf(depends_on, "%d �����ֶ�    ", sample_depends_on); break;
		default:
			sprintf(depends_on, "%d �����ֶ�    ", sample_depends_on); break;
			fprintf(g_mp4_log, "����:sample_depends_on δ֪ %d �ļ����� error %d\n", sample_depends_on,g_errors); g_errors++;
		}
		switch (sample_is_depended_on)
		{
		case 0: sprintf(is_depended_on, "%d ����֡����  ", sample_is_depended_on); break;
		case 1: sprintf(is_depended_on, "%d ������֡�ο�", sample_is_depended_on); break;
		case 2: sprintf(is_depended_on, "%d ����֡�ο�  ", sample_is_depended_on); break;
		case 3: sprintf(is_depended_on, "%d �����ֶ�    ", sample_is_depended_on); break;
		default:
			sprintf(is_depended_on, "%d �����ֶ�    ", sample_is_depended_on); break;
			fprintf(g_mp4_log, "����:sample_is_depended_on δ֪ %d �ļ����� error %d\n", sample_is_depended_on, g_errors); g_errors++;
		}
		switch (sample_has_redundancy)
		{
		case 0: sprintf(has_redundancy, "%d ������Ϣ����", sample_has_redundancy); break;
		case 1: sprintf(has_redundancy, "%d ӵ��������Ϣ", sample_has_redundancy); break;
		case 2: sprintf(has_redundancy, "%d û��������Ϣ", sample_has_redundancy); break;
		case 3: sprintf(has_redundancy, "%d �����ֶ�    ", sample_has_redundancy); break;
		default:
			sprintf(has_redundancy, "%d �����ֶ�    ", sample_has_redundancy); break;
			fprintf(g_mp4_log, "����:sample_has_redundancy δ֪ %d �ļ����� error %d\n", sample_has_redundancy, g_errors); g_errors++;
		}
		fprintf(g_mp4_log, "%5d  %s         %s     %s\n", entry, depends_on, is_depended_on, has_redundancy);
		mp4Info.trackInfor[mp4Info.current_track_ID].sdtp[entry].reserved = reserved;
		mp4Info.trackInfor[mp4Info.current_track_ID].sdtp[entry].sample_depends_on = sample_depends_on;
		mp4Info.trackInfor[mp4Info.current_track_ID].sdtp[entry].sample_is_depended_on = sample_is_depended_on;
		mp4Info.trackInfor[mp4Info.current_track_ID].sdtp[entry].sample_has_redundancy = sample_has_redundancy;
	}


	return 0;
}