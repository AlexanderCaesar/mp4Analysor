#ifndef  XIAOC_AVCC_BOX_H
#define  XIAOC_AVCC_BOX_H
//����  �����  20171231  xiaoc@pku.edu.cn

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

	unsigned char   configurationVersion; //Ӧ��Ϊ1
	unsigned char   AVCProfileIndication;//����
	unsigned char   profile_compatibility;//ռ��8λ ��ʾ���H.264���ĸ���¼��׼ 0��ʾ���޶�
	unsigned char   AVCLevelIndication;//���𣬶��嵱ǰ������ʵ���Ϣ
	unsigned char   reserved0;// ռ6λ��111111��b;
	unsigned char   lengthSizeMinusOne;//ռ2λ NALͷ����
	unsigned char   reserved1;//ռ3λ = ��111��b;
	unsigned char   numOfSequenceParameterSets;//ռ5λ SPS����
	unsigned char   numOfPictureParameterSets;//ռ5λ PPS����

	H264Util        h264;

	unsigned __int64 boxSize;  // Box������size

	unsigned __int64 read_bytes; //�Ѿ���ȡ���Լ���Ŀ

public:
	AVCCBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif