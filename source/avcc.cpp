//作者  蔡砚刚  20171231  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "avcc.h"
#include "box.h"
#include "H264_util.h"

AVCCBOX::AVCCBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目
}
#define  MAX_DATA_CHARS 256
int AVCCBOX::anlysis()
{
	unsigned char data[MAX_DATA_CHARS];
	
	fprintf(g_mp4_log, "\n++++++++++++++++++AVCCBOX 概要信息+++++++++++++++++++++\n");

	int ret = fread(data, 1, 1, g_mp4_file);
    if (ret !=1)
	{
		fprintf(g_mp4_log, "错误:configurationVersion不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	configurationVersion = data[0];
	fprintf(g_mp4_log, "configurationVersion:    %d 配置版本 占用8位  一定为1 当前值为：0x 01\n", configurationVersion);
	if (configurationVersion != 1)
	{
		fprintf(g_mp4_log, "错误:configurationVersion 应该为1 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 1;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "错误:AVCProfileIndication不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
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
		fprintf(g_mp4_log, "错误：未知profile error %d\n", g_errors); g_errors++;
		break;
	}
	read_bytes += 1;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "错误:profile_compatibility不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	profile_compatibility = data[0];
	fprintf(g_mp4_log, "profile_compatibility:   %d \n", profile_compatibility);
	read_bytes += 1;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "错误:AVCLevelIndication不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	AVCLevelIndication = data[0];
	fprintf(g_mp4_log, "AVCLevelIndication  :    %d 级别，定义当前最大码率等信息\n", AVCLevelIndication);
	read_bytes += 1;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "错误:reserved0不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved0 = data[0]>>2;
	fprintf(g_mp4_log, "reserved0           :    %d 保留字段\n", reserved0);
	if (reserved0 != 63)
	{
		fprintf(g_mp4_log, "错误:reserved0 应该为63 0x111111 error %d\n", g_errors); g_errors++;
	}

	lengthSizeMinusOne = data[0]&0x03;
	fprintf(g_mp4_log, "lengthSizeMinusOne  :    %d NAL头长度\n", lengthSizeMinusOne + 1);
	read_bytes += 1;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret !=1)
	{
		fprintf(g_mp4_log, "错误:reserved0不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved1 = data[0] >> 5;
	fprintf(g_mp4_log, "reserved1           :    %d 保留字段\n", reserved1);
	if (reserved1 != 7)
	{
		fprintf(g_mp4_log, "错误:reserved1 应该为7 0x111 error %d\n", g_errors); g_errors++;
	}

	numOfSequenceParameterSets = data[0] & 0x1F;
	fprintf(g_mp4_log, "numOfSequenceParameterSets:    %d SPS个数\n", numOfSequenceParameterSets);
	read_bytes += 1;

	for (int i = 0; i < numOfSequenceParameterSets; i++)
	{
		ret = fread(data, 1, 2, g_mp4_file);
		if (ret != 2)
		{
			fprintf(g_mp4_log, "错误:sequenceParameterSetLength不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		unsigned int sequenceParameterSetLength = (data[0] << 8) + (data[1] << 0);
		fprintf(g_mp4_log, "+++++++++++SPS [%d] +++++++++++\n", i);
		fprintf(g_mp4_log, "SPS长度:%d\n", sequenceParameterSetLength);
		read_bytes += 2;
		if (sequenceParameterSetLength > MAX_DATA_CHARS)
		{
			fprintf(g_mp4_log, "错误:sequenceParameterSetLength 大于 MAX_DATA_CHARS %d error %d\n", MAX_DATA_CHARS,g_errors); g_errors++;
			return -1;
		}
		ret = fread(data, 1, sequenceParameterSetLength, g_mp4_file);
		read_bytes += sequenceParameterSetLength;
		if (ret != sequenceParameterSetLength)
		{
			fprintf(g_mp4_log, "错误:sequenceParameterSetLength不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		int naltype = h264.ParseNaluType(data[0]);
		fprintf(g_mp4_log, "NALtype:%d %s\n", naltype,NaluTypeString[naltype]);
		if (naltype != NAL_SPS)
		{
			fprintf(g_mp4_log, "错误:naltype 必须为SPS error %d\n", g_errors); g_errors++;
			return -1;
		}
		uint32_t width = 0;
		uint32_t height = 0;
		if (h264.ParseSps(data, &width, &height))
		{
			fprintf(g_mp4_log, "width  :%d \n", width);
			fprintf(g_mp4_log, "height :%d \n", height);
		}
		if (m_param.b_video) //输出码流
		{
			unsigned char NALHeader[4] = { 0, 0, 0, 1 };
			
			fwrite(NALHeader, 1, 4, g_mp4_video);
			fwrite(data, 1, sequenceParameterSetLength, g_mp4_video);
		}
	}

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "错误:numOfPictureParameterSets不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	numOfPictureParameterSets = data[0];
	fprintf(g_mp4_log, "numOfPictureParameterSets:    %d PPS个数\n", numOfPictureParameterSets);
	read_bytes += 1;

	for (int i = 0; i < numOfPictureParameterSets; i++)
	{
		ret = fread(data, 1, 2, g_mp4_file);
		if (ret != 2)
		{
			fprintf(g_mp4_log, "错误:pictureParameterSetLength不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		unsigned int pictureParameterSetLength = (data[0] << 8) + (data[1] << 0);
		fprintf(g_mp4_log, "+++++++++++PPS [%d] +++++++++++\n", i);
		fprintf(g_mp4_log, "PPS长度:%d\n", pictureParameterSetLength);
		read_bytes += 2;
		if (pictureParameterSetLength > MAX_DATA_CHARS)
		{
			fprintf(g_mp4_log, "错误:pictureParameterSetLength 大于 MAX_DATA_CHARS %d error %d\n", MAX_DATA_CHARS, g_errors); g_errors++;
			return -1;
		}
		ret = fread(data, 1, pictureParameterSetLength, g_mp4_file);
		read_bytes += pictureParameterSetLength;
		if (ret != pictureParameterSetLength)
		{
			fprintf(g_mp4_log, "错误:pictureParameterSetLength不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		int naltype = h264.ParseNaluType(data[0]);
		fprintf(g_mp4_log, "NALtype:%d %s\n", naltype, NaluTypeString[naltype]);
		if (naltype != NAL_PPS)
		{
			fprintf(g_mp4_log, "错误:naltype 必须为PPS error %d\n", g_errors); g_errors++;
			return -1;
		}

		if (m_param.b_video) //输出码流
		{
			unsigned char NALHeader[4] = { 0, 0, 0, 1 };

			fwrite(NALHeader, 1, 4, g_mp4_video);
			fwrite(data, 1, pictureParameterSetLength, g_mp4_video);
		}
	}
	
	return 0;
}