#ifndef  XIAOC_MP4_STSTBOX_H
#define  XIAOC_MP4_STSTBOX_H
//����  �����  20171229  xiaoc@pku.edu.cn
#include "box.h"

class STSDBOX : public FullBox //DataReferenceBox
{
public:
	int          subsubboxConter;//����EDTSBOX�е�subbox����
	unsigned int entry_count;    //���������
public:
	STSDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};



class SampleEntry : public BOX{

public:
	unsigned __int64 reserved0; //ռ48λ
	/*data_reference_index is an integer that contains 
	the index of the data reference to use to retrieve
    data associated with samples that use this sample
	description. Data references are stored in Data
     Reference Boxes. The index ranges from 1 to the number 
	 of data references.*/
	unsigned int  data_reference_index;//ռ16λ
};
#define MAX_OTHER_CHARS 256
class VisualSampleEntryBOX :public SampleEntry
{
public:

	unsigned int pre_defined;  //�ֶ�ռ16λ Ӧ��Ϊ0
	unsigned int reserved ;    //�����ֶ�ռ16λ Ӧ��Ϊ0
	unsigned int pre_defined1[3];//�ֶ�ռ32*3λ Ӧ��Ϊ0
	unsigned int width;          //��Ƶ���ռ16λ
	unsigned int height;         //��Ƶ�߶�ռ16λ
	unsigned int horizresolution; //ռ32λ = 0x00480000; // 72 dpi 
	unsigned int vertresolution;  //ռ32λ = 0x00480000;  // 72 dpi 
	unsigned int reserved2;       //�����ֶ�ռ32λ Ӧ��Ϊ0
	unsigned int frame_count;     //ռ16λ ÿ���������е���Ƶ֡�� Ĭ��Ϊ1
	/*Compressorname is a name, for informative purposes.
	  It is formatted in a fixed 32 - byte field, with the
	  first byte set to the number of bytes to be displayed, 
	  followed by that number of bytes of displayable
	  data, and then padding to complete 32 bytes total
	  (including the size byte).The field may be set to 0.*/
	unsigned char compressorname[32]; //ռ32*8λ
	unsigned int  depth;          //ռ16λ = 0x0018; ռ32λ images are in colour with no alpha
	short         pre_defined2;   //ռ16λ Ӧ��Ϊ-1
	unsigned char others[MAX_OTHER_CHARS]; //other strings


public:
	VisualSampleEntryBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif