#ifndef  XIAOC_MP4_STBLBOX_H
#define  XIAOC_MP4_STBLBOX_H
//����  �����  20171229  xiaoc@pku.edu.cn
#include "box.h"

class STBLBOX: public BOX 
{
public:
	int        subsubboxConter;//����EDTSBOX�е�subbox����
public:
	STBLBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif