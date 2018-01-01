#ifndef  XIAOC_MP4_STSTBOX_H
#define  XIAOC_MP4_STSTBOX_H
//作者  蔡砚刚  20171229  xiaoc@pku.edu.cn
#include "box.h"

class STSDBOX : public FullBox //DataReferenceBox
{
public:
	int          subsubboxConter;//计数EDTSBOX中的subbox个数
	unsigned int entry_count;    //数据项个数
public:
	STSDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};



class SampleEntry : public BOX{

public:
	unsigned __int64 reserved0; //占48位
	/*data_reference_index is an integer that contains 
	the index of the data reference to use to retrieve
    data associated with samples that use this sample
	description. Data references are stored in Data
     Reference Boxes. The index ranges from 1 to the number 
	 of data references.*/
	unsigned int  data_reference_index;//占16位
};
#define MAX_OTHER_CHARS 256
class VisualSampleEntryBOX :public SampleEntry
{
public:

	unsigned int pre_defined;  //字段占16位 应该为0
	unsigned int reserved ;    //保留字段占16位 应该为0
	unsigned int pre_defined1[3];//字段占32*3位 应该为0
	unsigned int width;          //视频宽度占16位
	unsigned int height;         //视频高度占16位
	unsigned int horizresolution; //占32位 = 0x00480000; // 72 dpi 
	unsigned int vertresolution;  //占32位 = 0x00480000;  // 72 dpi 
	unsigned int reserved2;       //保留字段占32位 应该为0
	unsigned int frame_count;     //占16位 每个样本含有的视频帧数 默认为1
	/*Compressorname is a name, for informative purposes.
	  It is formatted in a fixed 32 - byte field, with the
	  first byte set to the number of bytes to be displayed, 
	  followed by that number of bytes of displayable
	  data, and then padding to complete 32 bytes total
	  (including the size byte).The field may be set to 0.*/
	unsigned char compressorname[32]; //占32*8位
	unsigned int  depth;          //占16位 = 0x0018; 占32位 images are in colour with no alpha
	short         pre_defined2;   //占16位 应该为-1
	unsigned char others[MAX_OTHER_CHARS]; //other strings


public:
	VisualSampleEntryBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif