//作者  蔡砚刚  20180102  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "esds.h"

char *ObjectProfileIndicationType[]
{
	"0x00 Reserved for ISO use",
		"0x01 Systems 14496 - 1 Simple Scene Description",
		"0x02 Systems 14496 - 1 2D Scene Description",
		"0x03 Systems 14496 - 1 VRML Scene Description",
		"0x04 Systems 14496 - 1 Audio Scene Description",
		"0x05 Systems 14496 - 1 Complete Scene Description",
		"0x06 - ",
		"none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none",
		"none", "none", "none", "none", "none", "none",
		"0x1F reserved for ISO use",
		"0x20 Visual 14496 - 2 simple profile",
		"0x21 Visual 14496 - 2 core profile",
		"0x22 Visual 14496 - 2 main profile",
		"0x23 Visual 14496 - 2 simple scalable profile",
		"0x24 Visual 14496 - 2 12 - Bit",
		"0x25 Visual 14496 - 2 Basic Anim. 2D Texture",
		"0x26 Visual 14496 - 2 Anim. 2D Mesh",
		"0x27 Visual 14496 - 2 Simple Face ",
		"0x28 Visual 14496 - 2 Simple Scalable Texture" ,
		"0x29 Visual 14496 - 2 Core Scalable Texture" ,
		"0x2A - " ,
		"none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none", "none",
		"none", "none", "none", "none", "none", "none", "none", 
		"0x3F reserved for ISO use" ,
		"0x40 Audio 14496 - 3 AAC Main" ,
		"0x41 Audio 14496 - 3 AAC LC" ,
		"0x42 Audio 14496 - 3 T / F" ,
		"0x43 Audio 14496 - 3 T / F Main scalable" ,
		"0x44 Audio 14496 - 3 T / F LC scalable" ,
		"0x45 Audio 14496 - 3 Twin VQ core" ,
		"0x46 Audio 14496 - 3 CELP" ,
		"0x47 Audio 14496 - 3 HVXC" ,
		"0x48 Audio 14496 - 3 HILN" ,
		"0x49 Audio 14496 - 3 TTSI" ,
		"0x4A Audio 14496 - 3 Main Synthetic" ,
		"0x4B Audio 14496 - 3 Wavetable Synthesis" ,
		"0x4C - " ,
		"none", "none", "none", "none", "none", "none", "none", "none", "none", "none",
		"none", "none", "none", "none", "none", "none", "none", "none", "none",
		"0x5F reserved for ISO use" ,
		"0x60 Visual 13818 - 2 Simple Profile" ,
		"0x61 Visual 13818 - 2 Main Profile" ,
		"0x62 Visual 13818 - 2 SNR Profile" ,
		"0x63 Visual 13818 - 2 Spatial Profile" ,
		"0x64 Visual 13818 - 2 High Profile" ,
		"0x65 Visual 13818 - 2 422 Profile" ,
		"0x66 Audio 13818 - 7" ,
		"0x67 Audio 13818 - 3" ,
		"0x68 Visual 11172 - 2" ,
		"0x69 Audio 11172 - 3" ,
		"0x6A - " ,
};

int read_esds_len(unsigned __int64 &read_bytes)
{
	int len = 0;
	int count = 4;
	unsigned char data[4];
	while (count--) {
		fread(data, 1, 1, g_mp4_file);
		read_bytes++;
		int c = (int)data[0];
		len = (len << 7) | (c & 0x7f);
		if (!(c & 0x80))
			break;
	}
	return len;
}

ESDSBOX::ESDSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目
}
#define  MAX_DATA_ESDS_CHARS 256
int ESDSBOX::anlysis()
{
	unsigned char data[MAX_DATA_ESDS_CHARS];
	
	fprintf(g_mp4_log, "\n++++++++++++++++++ESDSTAG 概要信息+++++++++++++++++++++\n");

	int ret = fread(&version, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "错误:BOX version不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "version              :    %d\n", version);

	ret = fread(data, 1, 3, g_mp4_file);
	if (ret != 3)
	{
		fprintf(g_mp4_log, "错误:flag不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	flag = (((unsigned int)data[0]) << 16) + (((unsigned int)data[1]) << 8) + (((unsigned int)data[2]) << 0);
	fprintf(g_mp4_log, "flag                 :    %u\n", flag);

	read_bytes += 4;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "错误:tag不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	eSDescrTag.tag = data[0];
	fprintf(g_mp4_log, "tag                  :    %d ", eSDescrTag.tag);
	switch (eSDescrTag.tag)
	{
	case MP4ODescrTag:fprintf(g_mp4_log, "MP4ODescrTag\n"); break;
	case MP4IODescrTag: fprintf(g_mp4_log, "MP4IODescrTag\n"); break;
	case MP4ESDescrTag:fprintf(g_mp4_log, "MP4ESDescrTag\n"); break;
	case MP4DecConfigDescrTag:fprintf(g_mp4_log, "MP4DecConfigDescrTag\n"); break;
	case MP4DecSpecificDescrTag:fprintf(g_mp4_log, "MP4DecSpecificDescrTag\n"); break;
	case MP4SLDescrTag: fprintf(g_mp4_log, "MP4SLDescrTag\n"); break;
	default:
		fprintf(g_mp4_log, "错误:tag类型待分析 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 1;

	eSDescrTag.size = read_esds_len(read_bytes);
	fprintf(g_mp4_log, "size                 :    %d 不包括tag头长度\n", eSDescrTag.size);

	switch (eSDescrTag.tag)
	{
	case MP4ESDescrTag: break;
	default:
		fprintf(g_mp4_log, "错误:tag类型待分析 error %d\n", g_errors); g_errors++;
		ret = fseek(g_mp4_file, (long)(eSDescrTag.size), SEEK_CUR);
		read_bytes += eSDescrTag.size;
		if (ret < 0)
		{
			fprintf(g_mp4_log, "错误:文件可能有损坏error %d\n", g_errors); g_errors++;
		}
		return 0;
	}
	ret = fread(data, 1, 2, g_mp4_file);
	if (ret != 2)
	{
		fprintf(g_mp4_log, "错误:ES_ID不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	eSDescrTag.ES_ID = (data[0] << 8) + (data[1] << 0);
	fprintf(g_mp4_log, "ES_ID                :    %d \n", eSDescrTag.ES_ID);
	read_bytes += 2;

	ret = fread(data, 1, 1, g_mp4_file);
	if (ret != 1)
	{
		fprintf(g_mp4_log, "错误:ES_ID不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}
	eSDescrTag.streamDependenceFlag = data[0] & 0x80;
	fprintf(g_mp4_log, "streamDependenceFlag :    %d \n", eSDescrTag.streamDependenceFlag);
	eSDescrTag.URL_Flag = data[0] & 0x40;
	fprintf(g_mp4_log, "URL_Flag             :    %d \n", eSDescrTag.URL_Flag);
	eSDescrTag.reserve = data[0] & 0x20;
	fprintf(g_mp4_log, "OCRstreamFlag        :    %d \n", eSDescrTag.reserve);
	eSDescrTag.streamPriority = data[0] & 0x1F;
	fprintf(g_mp4_log, "streamPriority       :    %d \n", eSDescrTag.streamPriority);
	read_bytes += 1;

	if (eSDescrTag.streamDependenceFlag)
	{
		ret = fread(data, 1, 2, g_mp4_file);
		if (ret != 2)
		{
			fprintf(g_mp4_log, "错误:dependsOn_ES_ID不够2字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		eSDescrTag.dependsOn_ES_ID = (data[0] << 8) + (data[1] << 0);
		fprintf(g_mp4_log, "dependsOn_ES_ID       :    %d \n", eSDescrTag.dependsOn_ES_ID);
		read_bytes += 2;
	}

	if (eSDescrTag.URL_Flag)
	{
		int urlsize = eSDescrTag.size - 3 - (eSDescrTag.streamDependenceFlag * 2);
		if (urlsize >= MAX_DATA_ESDS_CHARS)
		{
			fprintf(g_mp4_log, "错误:url length 大于MAX_DATA_ESDS_CHARS %d error %d\n", MAX_DATA_ESDS_CHARS,g_errors); g_errors++;
			return -1;
		}
		data[urlsize] = '\0';
		fprintf(g_mp4_log, "url                   :    %d \n", data);
		fprintf(g_mp4_log, "错误:请仔细分析\n", MAX_DATA_ESDS_CHARS, g_errors); g_errors++;
		read_bytes += urlsize;
		return -1;
	}

	//分析decConfigDescrTag
	{
		ret = fread(data, 1, 1, g_mp4_file);
		if (ret != 1)
		{
			fprintf(g_mp4_log, "错误:tag不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		decConfigDescrTag.tag = data[0];
		fprintf(g_mp4_log, "+++tag                  :    %d ", decConfigDescrTag.tag);
		switch (decConfigDescrTag.tag)
		{
		case MP4ODescrTag:fprintf(g_mp4_log, "MP4ODescrTag\n"); break;
		case MP4IODescrTag: fprintf(g_mp4_log, "MP4IODescrTag\n"); break;
		case MP4ESDescrTag:fprintf(g_mp4_log, "MP4ESDescrTag\n"); break;
		case MP4DecConfigDescrTag:fprintf(g_mp4_log, "MP4DecConfigDescrTag\n"); break;
		case MP4DecSpecificDescrTag:fprintf(g_mp4_log, "MP4DecSpecificDescrTag\n"); break;
		case MP4SLDescrTag: fprintf(g_mp4_log, "MP4SLDescrTag\n"); break;
		default:
			fprintf(g_mp4_log, "错误:tag类型待分析 error %d\n", g_errors); g_errors++;
		}
		read_bytes += 1;

		decConfigDescrTag.size = read_esds_len(read_bytes);
		fprintf(g_mp4_log, "+++size                 :    %d 不包括tag头长度\n", decConfigDescrTag.size);

		switch (decConfigDescrTag.tag)
		{
		case MP4DecConfigDescrTag: break;
		default:
			fprintf(g_mp4_log, "错误:tag类型待分析 error %d\n", g_errors); g_errors++;
			ret = fseek(g_mp4_file, (long)(decConfigDescrTag.size), SEEK_CUR);
			read_bytes += decConfigDescrTag.size;
			if (ret < 0)
			{
				fprintf(g_mp4_log, "错误:文件可能有损坏error %d\n", g_errors); g_errors++;
			}
		}

		ret = fread(data, 1, 1, g_mp4_file);
		if (ret != 1)
		{
			fprintf(g_mp4_log, "错误:objectProfileIndication不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		decConfigDescrTag.objectProfileIndication = data[0];
		fprintf(g_mp4_log, "+++objectProfileIndication:    %d 0x%x ", decConfigDescrTag.objectProfileIndication, decConfigDescrTag.objectProfileIndication);
		if (decConfigDescrTag.objectProfileIndication >0x6A)
		{
			fprintf(g_mp4_log, "错误:objectProfileIndication 不在标准内 error %d\n", g_errors); g_errors++;
			return -1;
		}
		fprintf(g_mp4_log, " %s\n", ObjectProfileIndicationType[decConfigDescrTag.objectProfileIndication]);
		read_bytes += 1;

		ret = fread(data, 1, 1, g_mp4_file);
		if (ret != 1)
		{
			fprintf(g_mp4_log, "错误:streamType不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		decConfigDescrTag.streamType = data[0]&0xFC;
		fprintf(g_mp4_log, "+++streamType           :    %d 0x%x\n", decConfigDescrTag.streamType, decConfigDescrTag.streamType);

		decConfigDescrTag.upStream = data[0] & 0x02;
		fprintf(g_mp4_log, "+++upStream             :    %d \n", decConfigDescrTag.upStream);

		decConfigDescrTag.reserved = data[0] & 0x01;
		fprintf(g_mp4_log, "+++reserved             :    %d \n", decConfigDescrTag.reserved);
		read_bytes += 1;

		ret = fread(data, 1, 3, g_mp4_file);
		if (ret != 3)
		{
			fprintf(g_mp4_log, "错误:bufferSizeDB不够3字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		decConfigDescrTag.bufferSizeDB = (((unsigned int)data[0]) << 16) + (((unsigned int)data[1]) << 8) + (((unsigned int)data[2]) << 0);
		fprintf(g_mp4_log, "+++bufferSizeDB         :    %d \n", decConfigDescrTag.bufferSizeDB);
		read_bytes += 3;

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "错误:maxBitrate不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		decConfigDescrTag.maxBitrate = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "+++maxBitrate           :    %d \n", decConfigDescrTag.maxBitrate);
		read_bytes += 4;

		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "错误:avgBitrate不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
			return -1;
		}
		decConfigDescrTag.avgBitrate = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "+++avgBitrate           :    %d \n", decConfigDescrTag.avgBitrate);
		read_bytes += 4;

		//DecoderSpecificInfo
		{
			ret = fread(data, 1, 1, g_mp4_file);
			if (ret != 1)
			{
				fprintf(g_mp4_log, "错误:tag不够1字节 文件有损坏 error %d\n", g_errors); g_errors++;
				return -1;
			}
			decoderSpecificInfo.tag = data[0];
			fprintf(g_mp4_log, "+++++tag                  :    %d ", decoderSpecificInfo.tag);
			switch (decoderSpecificInfo.tag)
			{
			case MP4ODescrTag:fprintf(g_mp4_log, "MP4ODescrTag\n"); break;
			case MP4IODescrTag: fprintf(g_mp4_log, "MP4IODescrTag\n"); break;
			case MP4ESDescrTag:fprintf(g_mp4_log, "MP4ESDescrTag\n"); break;
			case MP4DecConfigDescrTag:fprintf(g_mp4_log, "MP4DecConfigDescrTag\n"); break;
			case MP4DecSpecificDescrTag:fprintf(g_mp4_log, "MP4DecSpecificDescrTag\n"); break;
			case MP4SLDescrTag: fprintf(g_mp4_log, "MP4SLDescrTag\n"); break;
			default:
				fprintf(g_mp4_log, "错误:tag类型待分析 error %d\n", g_errors); g_errors++;
			}
			read_bytes += 1;

			decoderSpecificInfo.size = read_esds_len(read_bytes);
			fprintf(g_mp4_log, "+++++size                 :    %d 不包括tag头长度\n", decoderSpecificInfo.size);

			ret = fseek(g_mp4_file, (long)(decoderSpecificInfo.size), SEEK_CUR);
			read_bytes += decoderSpecificInfo.size;
			if (ret < 0)
			{
				fprintf(g_mp4_log, "错误:文件可能有损坏error %d\n", g_errors); g_errors++;
			}
		}
		
	}//分析decConfigDescrTag
	ret = fseek(g_mp4_file, (long)(boxSize-read_bytes), SEEK_CUR);
	if (ret < 0)
	{
		fprintf(g_mp4_log, "错误:文件可能有损坏error %d\n", g_errors); g_errors++;
	}
	read_bytes += boxSize - read_bytes;
	return 0;
}