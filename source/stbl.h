#ifndef  XIAOC_MP4_STBLBOX_H
#define  XIAOC_MP4_STBLBOX_H
//作者  蔡砚刚  20171229  xiaoc@pku.edu.cn
#include "box.h"

class STBLBOX: public BOX 
{
public:
	int        subsubboxConter;//计数EDTSBOX中的subbox个数
public:
	STBLBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif