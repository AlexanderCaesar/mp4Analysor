//����  �����  20171231  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "avcc.h"
#include "box.h"
#include "H264_util.h"

AVCCBOX::AVCCBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ
}
#define  MAX_DATA_CHARS 256
int AVCCBOX::anlysis()
{
	unsigned char data[MAX_DATA_CHARS];
	
	fprintf(g_mp4_log, "\n++++++++++++++++++AVCCBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");

	int ret = fread(data, 1, 1, g_mp4_file);
    if (ret !=1)
	{
		fprintf(g_mp4_log, "����:configurationVersion����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	configurationVersion = data[0];
	fprintf(g_mp4_log, "configurationVersion:    %d ���ð汾 ռ��8λ  һ��Ϊ1 ��ǰֵΪ��0x 01\n", configurationVersion);
	if (configurationVersion != 1)
	{
		fprintf(g_mp4_log, "����:configurationVersion Ӧ��Ϊ1 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 1;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "����:AVCProfileIndication����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	AVCProfileIndication = data[0];
	fprintf(g_mp4_log, "AVCProfileIndication:    %d 0x%x ", AVCProfileIndication, AVCProfileIndication);
	switch (AVCProfileIndication)
	{
	case PROFILE_BASELINE:fprintf(g_mp4_log, "PROFILE_BASELINE\n"); break;
	case PROFILE_MAIN:fprintf(g_mp4_log, "PROFILE_MAIN\n"); break;
	case PROFILE_HIGH:fprintf(g_mp4_log, "PROFILE_HIGH\n"); break;
	case PROFILE_HIGH10:fprintf(g_mp4_log, "PROFILE_HIGH10\n"); break;
	case PROFILE_HIGH422:fprintf(g_mp4_log, "PROFILE_HIGH422\n"); break;
	case PROFILE_HIGH444_PREDICTIVE:fprintf(g_mp4_log, "PROFILE_HIGH444_PREDICTIVE\n"); break;

	default:
		fprintf(g_mp4_log, "����δ֪profile error %d\n", g_errors); g_errors++;
		break;
	}
	read_bytes += 1;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "����:profile_compatibility����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	profile_compatibility = data[0];
	fprintf(g_mp4_log, "profile_compatibility:   %d \n", profile_compatibility);
	read_bytes += 1;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "����:AVCLevelIndication����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	AVCLevelIndication = data[0];
	fprintf(g_mp4_log, "AVCLevelIndication  :    %d ���𣬶��嵱ǰ������ʵ���Ϣ\n", AVCLevelIndication);
	read_bytes += 1;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "����:reserved0����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved0 = data[0]>>2;
	fprintf(g_mp4_log, "reserved0           :    %d �����ֶ�\n", reserved0);
	if (reserved0 != 63)
	{
		fprintf(g_mp4_log, "����:reserved0 Ӧ��Ϊ63 0x111111 error %d\n", g_errors); g_errors++;
	}

	lengthSizeMinusOne = data[0]&0x03;
	fprintf(g_mp4_log, "lengthSizeMinusOne  :    %d NALͷ����\n", lengthSizeMinusOne + 1);
	read_bytes += 1;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret !=1)
	{
		fprintf(g_mp4_log, "����:reserved0����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved1 = data[0] >> 5;
	fprintf(g_mp4_log, "reserved1           :    %d �����ֶ�\n", reserved1);
	if (reserved1 != 7)
	{
		fprintf(g_mp4_log, "����:reserved1 Ӧ��Ϊ7 0x111 error %d\n", g_errors); g_errors++;
	}

	numOfSequenceParameterSets = data[0] & 0x1F;
	fprintf(g_mp4_log, "numOfSequenceParameterSets:    %d SPS����\n", numOfSequenceParameterSets);
	read_bytes += 1;

	for (int i = 0; i < numOfSequenceParameterSets; i++)
	{
		ret = fread(data, 1, 2, g_mp4_file);
		if (ret != 2)
		{
			fprintf(g_mp4_log, "����:sequenceParameterSetLength����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		unsigned int sequenceParameterSetLength = (data[0] << 8) + (data[1] << 0);
		fprintf(g_mp4_log, "+++++++++++SPS [%d] +++++++++++\n", i);
		fprintf(g_mp4_log, "SPS����:%d\n", sequenceParameterSetLength);
		read_bytes += 2;
		if (sequenceParameterSetLength > MAX_DATA_CHARS)
		{
			fprintf(g_mp4_log, "����:sequenceParameterSetLength ���� MAX_DATA_CHARS %d error %d\n", MAX_DATA_CHARS,g_errors); g_errors++;
			return -1;
		}
		ret = fread(data, 1, sequenceParameterSetLength, g_mp4_file);
		read_bytes += sequenceParameterSetLength;
		if (ret != sequenceParameterSetLength)
		{
			fprintf(g_mp4_log, "����:sequenceParameterSetLength����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		int naltype = h264.ParseNaluType(data[0]);
		fprintf(g_mp4_log, "NALtype:%d %s\n", naltype,NaluTypeString[naltype]);
		if (naltype != NAL_SPS)
		{
			fprintf(g_mp4_log, "����:naltype ����ΪSPS error %d\n", g_errors); g_errors++;
			return -1;
		}
		uint32_t width = 0;
		uint32_t height = 0;
		if (h264.ParseSps(data, &width, &height))
		{
			fprintf(g_mp4_log, "width  :%d \n", width);
			fprintf(g_mp4_log, "height :%d \n", height);
		}
		if (m_param.b_video) //�������
		{
			unsigned char NALHeader[4] = { 0, 0, 0, 1 };
			
			fwrite(NALHeader, 1, 4, g_mp4_video);
			fwrite(data, 1, sequenceParameterSetLength, g_mp4_video);
		}
	}

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "����:numOfPictureParameterSets����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	numOfPictureParameterSets = data[0];
	fprintf(g_mp4_log, "numOfPictureParameterSets:    %d PPS����\n", numOfPictureParameterSets);
	read_bytes += 1;

	for (int i = 0; i < numOfPictureParameterSets; i++)
	{
		ret = fread(data, 1, 2, g_mp4_file);
		if (ret != 2)
		{
			fprintf(g_mp4_log, "����:pictureParameterSetLength����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		unsigned int pictureParameterSetLength = (data[0] << 8) + (data[1] << 0);
		fprintf(g_mp4_log, "+++++++++++PPS [%d] +++++++++++\n", i);
		fprintf(g_mp4_log, "PPS����:%d\n", pictureParameterSetLength);
		read_bytes += 2;
		if (pictureParameterSetLength > MAX_DATA_CHARS)
		{
			fprintf(g_mp4_log, "����:pictureParameterSetLength ���� MAX_DATA_CHARS %d error %d\n", MAX_DATA_CHARS, g_errors); g_errors++;
			return -1;
		}
		ret = fread(data, 1, pictureParameterSetLength, g_mp4_file);
		read_bytes += pictureParameterSetLength;
		if (ret != pictureParameterSetLength)
		{
			fprintf(g_mp4_log, "����:pictureParameterSetLength����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		int naltype = h264.ParseNaluType(data[0]);
		fprintf(g_mp4_log, "NALtype:%d %s\n", naltype, NaluTypeString[naltype]);
		if (naltype != NAL_PPS)
		{
			fprintf(g_mp4_log, "����:naltype ����ΪPPS error %d\n", g_errors); g_errors++;
			return -1;
		}

		if (m_param.b_video) //�������
		{
			unsigned char NALHeader[4] = { 0, 0, 0, 1 };

			fwrite(NALHeader, 1, 4, g_mp4_video);
			fwrite(data, 1, pictureParameterSetLength, g_mp4_video);
		}
	}
	
	return 0;
}