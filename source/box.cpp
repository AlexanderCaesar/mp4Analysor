//作者  蔡砚刚  20171214  xiaoc@pku.edu.cn
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mp4file.h"
#include "box.h"

#include "ftyp.h"
#include "moov.h"
#include "mvhd.h"
#include "trak.h"
#include "trhd.h"
#include "edts.h"
#include "elst.h"
#include "mdia.h"
#include "mdhd.h"
#include "hdlr.h"
#include "minf.h"
#include "vmhd.h"
#include "wide.h"
#include "smhd.h"
#include "dinf.h"
#include "dref.h"
#include "url.h"
#include "stbl.h"
#include "stsd.h"
#include "avcc.h"
#include "esds.h"

#pragma warning(disable:4996)

BOX::BOX()
{
	size = 0;
	type.i_type = 0;
	largesize = 0;
	read_bytes = 0;
}

int BOX::readHeader()
{
	unsigned char data[12];
	int ret = fread(data, 1, 4, g_mp4_file); //read size
	if (ret == 0)
	{
		fprintf(g_mp4_log, "文件读取完毕\n"); ;
		return -1;
	}
	else if (ret != 4)
	{
		fprintf(g_mp4_log, "错误:BOX长度不够4字节 文件有损坏 error %d\n",g_errors); g_errors++;
		return -1;
	}
	size = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);

	if (size == 0 || size == 1)
	{
		fprintf(g_mp4_log, "错误:暂时未遇到该类场景 Boxsize为 0 或者 1 size: %d  error %d\n", size,g_errors); g_errors++;
		return -1;
	}

	boxSize = size;
	ret = fread(type.c_type, 1, 4, g_mp4_file); //read size

	if (ret != 4)
	{
		fprintf(g_mp4_log, "错误:BOX type 不够4字节 文件有损坏 error %d\n", g_errors); g_errors++;
		return -1;
	}

	fprintf(g_mp4_log, "BOX    长度:    %lld\n", boxSize);
	fprintf(g_mp4_log, "BOX    类型:    %c%c%c%c (%d)\n", type.c_type[0], type.c_type[1], type.c_type[2], type.c_type[3],type.i_type);

	read_bytes = 4 + 4;
	return 0;
}
int BOX::anlysis()
{
	int ret = 0;
	switch (type.i_type)
	{
	case FTYP:
		fprintf(g_mp4_log, "BOX    描述:    描述mp4文件全文件中一般唯一一个\n");
		{
			FTYPBOX ftypbox(boxSize,read_bytes);
			ret = ftypbox.anlysis();
			read_bytes = ftypbox.read_bytes;
		}
		break;
	case MOOV:
		fprintf(g_mp4_log, "BOX    描述:    描述媒体信息中一般唯一一个\n");
		{
			MOOVBOX moovbox(boxSize, read_bytes);
			ret = moovbox.anlysis();
			read_bytes = moovbox.read_bytes;
		}
		break;
	case MVHD:
		fprintf(g_mp4_log, "BOX    描述:    Movie Header,描述文件的总体信息，如：时长，创建时间等，一般唯一一个\n");
		{
			MVHDBOX mvhdbox(boxSize, read_bytes);
			ret = mvhdbox.anlysis();
			read_bytes = mvhdbox.read_bytes;
		}
		break;
	case TRAK:
		fprintf(g_mp4_log, "BOX    描述:    存放音频或者视频数据流信息\n");
		{
			TRAKBOX trakbox(boxSize, read_bytes);
			ret = trakbox.anlysis();
			read_bytes = trakbox.read_bytes;
		}
		break;

	case TRHD:
		fprintf(g_mp4_log, "BOX    描述:    描述当前轨道音频或者视频数据信息，如时长、宽高等\n");
		{
			TRHDBOX trhdbox(boxSize, read_bytes);
			ret = trhdbox.anlysis();
			read_bytes = trhdbox.read_bytes;
		}
		break;

	case EDTS:
		fprintf(g_mp4_log, "BOX    描述:    描述播放速率播放起始时间等 Edit BOX 可选BOX\n");
		{
			EDTSBOX edtsbox(boxSize, read_bytes);
			ret = edtsbox.anlysis();
			read_bytes = edtsbox.read_bytes;
		}
		break;

	case ELST:
		fprintf(g_mp4_log, "BOX    描述:    描述播放速率播放起始时间等 Edit List BOX 可选BOX\n");
		{
			ELSTBOX elstbox(boxSize, read_bytes);
			ret = elstbox.anlysis();
			read_bytes = elstbox.read_bytes;
		}
		break;

	case MDIA:
		fprintf(g_mp4_log, "BOX    描述:    轨道媒体信息\n");
		{
			MDIABOX mdiabox(boxSize, read_bytes);
			ret = mdiabox.anlysis();
			read_bytes = mdiabox.read_bytes;
		}
		break;
	case MDHD:
		fprintf(g_mp4_log, "BOX    描述:    轨道媒体的头信息，如当前轨道的timescale计算等\n");
		{
			MDHDBOX mdhdbox(boxSize, read_bytes);
			ret = mdhdbox.anlysis();
			read_bytes = mdhdbox.read_bytes;
		}
		break;

	case HDLR:
		fprintf(g_mp4_log, "BOX    描述:    轨道媒体的头信息，识别当前轨道为音频、视频等 Handler Reference Box\n");
		{
			HDLRBOX hdlrbox(boxSize, read_bytes);
			ret = hdlrbox.anlysis();
			read_bytes = hdlrbox.read_bytes;
		}
		break;
	case MINF:
		fprintf(g_mp4_log, "BOX    描述:    轨道媒体的信息 Media Information Box\n");
		{
			MINFBOX minfbox(boxSize, read_bytes);
			ret = minfbox.anlysis();
			read_bytes = minfbox.read_bytes;
		}
		break;

	case VMHD:
		fprintf(g_mp4_log, "BOX    描述:    视频相关的信息 Video Media Header Box\n");
		{
			VMHDBOX vmhdbox(boxSize, read_bytes);
			ret = vmhdbox.anlysis();
			read_bytes = vmhdbox.read_bytes;
		}
		break;

	case SMHD:
		fprintf(g_mp4_log, "BOX    描述:    音频相关的信息 Sound Media Header Box\n");
		{
			SMHDBOX smhdbox(boxSize, read_bytes);
			ret = smhdbox.anlysis();
			read_bytes = smhdbox.read_bytes;
		}
		break;

	case WIDE:
		fprintf(g_mp4_log, "BOX    描述:    Wide 在一个视频中见过一次 具体不明 Box\n");
		{
			WIDEBOX widebox(boxSize, read_bytes);
			ret = widebox.anlysis();
			read_bytes = widebox.read_bytes;
		}
		break;

	case DINF:
		fprintf(g_mp4_log, "BOX    描述:    数据信息Data Information Box\n");
		{
			DINFBOX dinfbox(boxSize, read_bytes);
			ret = dinfbox.anlysis();
			read_bytes = dinfbox.read_bytes;
		}
		break;

	case DREF:
		fprintf(g_mp4_log, "BOX    描述:    数据信息DataReferenceBox\n");
		{
			DREFBOX drefbox(boxSize, read_bytes);
			ret = drefbox.anlysis();
			read_bytes = drefbox.read_bytes;
		}
		break;

	case URL:
		fprintf(g_mp4_log, "BOX    描述:    数据信息URL\n");
		{
			URLBOX urlbox(boxSize, read_bytes);
			ret = urlbox.anlysis();
			read_bytes = urlbox.read_bytes;
		}
		break;
	case STBL:
		fprintf(g_mp4_log, "BOX    描述:    存放时间偏移信息等\n");
		{
			STBLBOX stblbox(boxSize, read_bytes);
			ret = stblbox.anlysis();
			read_bytes = stblbox.read_bytes;
		}
		break;
	case STSD:
		fprintf(g_mp4_log, "BOX    描述:    存放codec信息等\n");
		{
			STSDBOX stsdbox(boxSize, read_bytes);
			ret = stsdbox.anlysis();
			read_bytes = stsdbox.read_bytes;
		}
		break;
	case AVCC:
		fprintf(g_mp4_log, "BOX    描述:    存放SPS PPS等\n");
		{
			AVCCBOX avccbox(boxSize, read_bytes);
			ret = avccbox.anlysis();
			read_bytes = avccbox.read_bytes;
		}
		break;

	case ESDS:
		fprintf(g_mp4_log, "BOX    描述:    存放音频头信息等\n");
		{
			ESDSBOX esdsbox(boxSize, read_bytes);
			ret = esdsbox.anlysis();
			read_bytes = esdsbox.read_bytes;
		}
		break;
	default:
		ret = fseek(g_mp4_file, (long)(boxSize - read_bytes), SEEK_CUR);
		//fprintf(g_mp4_log, "错误:BOX type 类型未见过（待添加到分析） error %d\n", g_errors); g_errors++;
		if (ret < 0)
		{
			fprintf(g_mp4_log, "错误:文件可能有损坏error %d\n", g_errors); g_errors++;
		}
		read_bytes = boxSize;
		break;
	}
	if (read_bytes != boxSize)
	{
		fprintf(g_mp4_log, "错误:boxSize %lld 未读取完毕 read_bytes %lld error %d \n", boxSize, read_bytes, g_errors); g_errors++;
		return -1;
	}
	return ret;
}

