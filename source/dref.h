#ifndef  XIAOC_MP4_DREFBOX_H
#define  XIAOC_MP4_DREFBOX_H
//作者  蔡砚刚  20171229  xiaoc@pku.edu.cn
#include "box.h"

class DREFBOX: public FullBox //DataReferenceBox
{
public:
	int          subsubboxConter;//计数EDTSBOX中的subbox个数
	unsigned int entry_count;    //数据项个数
public:
	DREFBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif