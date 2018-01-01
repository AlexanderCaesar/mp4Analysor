#ifndef  XIAOC_MP4_MINFBOX_H
#define  XIAOC_MP4_MINFBOX_H
//作者  蔡砚刚  20171227  xiaoc@pku.edu.cn
#include "box.h"

class MINFBOX: public BOX //Media Information Box
{
public:
	int        subsubboxConter;//计数EDTSBOX中的subbox个数
public:
	MINFBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif