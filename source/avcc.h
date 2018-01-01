#ifndef  XIAOC_AVCC_BOX_H
#define  XIAOC_AVCC_BOX_H
//作者  蔡砚刚  20171231  xiaoc@pku.edu.cn

#include "box.h"
#include "H264_util.h"
enum profile_e
{
	PROFILE_BASELINE = 66,
	PROFILE_MAIN = 77,
	PROFILE_HIGH = 100,
	PROFILE_HIGH10 = 110,
	PROFILE_HIGH422 = 122,
	PROFILE_HIGH444_PREDICTIVE = 244,
};

class AVCCBOX:public BOX
{
public:

	unsigned char   configurationVersion; //应该为1
	unsigned char   AVCProfileIndication;//档次
	unsigned char   profile_compatibility;//占用8位 表示遵从H.264的哪个附录标准 0表示不限定
	unsigned char   AVCLevelIndication;//级别，定义当前最大码率等信息
	unsigned char   reserved0;// 占6位‘111111’b;
	unsigned char   lengthSizeMinusOne;//占2位 NAL头长度
	unsigned char   reserved1;//占3位 = ‘111’b;
	unsigned char   numOfSequenceParameterSets;//占5位 SPS个数
	unsigned char   numOfPictureParameterSets;//占5位 PPS个数

	H264Util        h264;

	unsigned __int64 boxSize;  // Box的最终size

	unsigned __int64 read_bytes; //已经读取的自己数目

public:
	AVCCBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif