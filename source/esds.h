#ifndef  XIAOC_ESDS_BOX_H
#define  XIAOC_ESDS_BOX_H
//作者  蔡砚刚  20180102  xiaoc@pku.edu.cn

#include "box.h"


enum MP4ESDSTAG
{  
	MP4ODescrTag = 0x01,
	MP4IODescrTag = 0x02,
	MP4ESDescrTag = 0x03,
	MP4DecConfigDescrTag = 0x04,
	MP4DecSpecificDescrTag = 0x05,
	MP4SLDescrTag = 0x06,
};
struct DecoderSpecificInfo{ //14496-1. 8.3.5 DecoderSpecificInfo 
	unsigned char tag; //tag 类型
	int           size;//   长度前面缺省0x80  如 0x80 0x80 0x24 说明占用三个字节 长度 0x24  (36)
};
struct ESDescrTag{ //14496-1. 8.3.3 ES_Descriptor 
	unsigned char tag; //tag 类型
	int           size;//   长度前面缺省0x80  如 0x80 0x80 0x24 说明占用三个字节 长度 0x24  (36)

	unsigned char ES_ID;//占16位
	unsigned char streamDependenceFlag;//占1位
	unsigned char URL_Flag;//占1位
	unsigned char reserve;//占1位
	unsigned char streamPriority;//占5位
	unsigned char dependsOn_ES_ID;//占16位
};
struct DecoderConfigDescriptor
{
	unsigned char tag; //tag 类型
	int           size;//   长度前面缺省0x80  如 0x80 0x80 0x24 说明占用三个字节 长度 0x24  (36)
	unsigned char objectProfileIndication;//占8位
	unsigned char streamType;//占6位
	unsigned char upStream;//占1位
	unsigned char reserved;//占1位
	unsigned int  bufferSizeDB;//占24位
	unsigned int  maxBitrate;//占32位
	unsigned int  avgBitrate;//占32位
};
class ESDSBOX:public FullBox // 
{
public:

	ESDescrTag               eSDescrTag;
	DecoderConfigDescriptor  decConfigDescrTag;
	DecoderSpecificInfo      decoderSpecificInfo;

	unsigned char   configurationVersion; //应该为1
	unsigned char   AVCProfileIndication;//档次
	unsigned char   profile_compatibility;//占用8位 表示遵从H.264的哪个附录标准 0表示不限定
	unsigned char   AVCLevelIndication;//级别，定义当前最大码率等信息
	unsigned char   reserved0;// 占6位‘111111’b;
	unsigned char   lengthSizeMinusOne;//占2位 NAL头长度
	unsigned char   reserved1;//占3位 = ‘111’b;
	unsigned char   numOfSequenceParameterSets;//占5位 SPS个数
	unsigned char   numOfPictureParameterSets;//占5位 PPS个数


	unsigned __int64 boxSize;  // Box的最终size

	unsigned __int64 read_bytes; //已经读取的自己数目

public:
	ESDSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif