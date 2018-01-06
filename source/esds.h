#ifndef  XIAOC_ESDS_BOX_H
#define  XIAOC_ESDS_BOX_H
//����  �����  20180102  xiaoc@pku.edu.cn

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
	unsigned char tag; //tag ����
	int           size;//   ����ǰ��ȱʡ0x80  �� 0x80 0x80 0x24 ˵��ռ�������ֽ� ���� 0x24  (36)
};
struct ESDescrTag{ //14496-1. 8.3.3 ES_Descriptor 
	unsigned char tag; //tag ����
	int           size;//   ����ǰ��ȱʡ0x80  �� 0x80 0x80 0x24 ˵��ռ�������ֽ� ���� 0x24  (36)

	unsigned char ES_ID;//ռ16λ
	unsigned char streamDependenceFlag;//ռ1λ
	unsigned char URL_Flag;//ռ1λ
	unsigned char reserve;//ռ1λ
	unsigned char streamPriority;//ռ5λ
	unsigned char dependsOn_ES_ID;//ռ16λ
};
struct DecoderConfigDescriptor
{
	unsigned char tag; //tag ����
	int           size;//   ����ǰ��ȱʡ0x80  �� 0x80 0x80 0x24 ˵��ռ�������ֽ� ���� 0x24  (36)
	unsigned char objectProfileIndication;//ռ8λ
	unsigned char streamType;//ռ6λ
	unsigned char upStream;//ռ1λ
	unsigned char reserved;//ռ1λ
	unsigned int  bufferSizeDB;//ռ24λ
	unsigned int  maxBitrate;//ռ32λ
	unsigned int  avgBitrate;//ռ32λ
};
class ESDSBOX:public FullBox // 
{
public:

	ESDescrTag               eSDescrTag;
	DecoderConfigDescriptor  decConfigDescrTag;
	DecoderSpecificInfo      decoderSpecificInfo;

	unsigned char   configurationVersion; //Ӧ��Ϊ1
	unsigned char   AVCProfileIndication;//����
	unsigned char   profile_compatibility;//ռ��8λ ��ʾ���H.264���ĸ���¼��׼ 0��ʾ���޶�
	unsigned char   AVCLevelIndication;//���𣬶��嵱ǰ������ʵ���Ϣ
	unsigned char   reserved0;// ռ6λ��111111��b;
	unsigned char   lengthSizeMinusOne;//ռ2λ NALͷ����
	unsigned char   reserved1;//ռ3λ = ��111��b;
	unsigned char   numOfSequenceParameterSets;//ռ5λ SPS����
	unsigned char   numOfPictureParameterSets;//ռ5λ PPS����


	unsigned __int64 boxSize;  // Box������size

	unsigned __int64 read_bytes; //�Ѿ���ȡ���Լ���Ŀ

public:
	ESDSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif