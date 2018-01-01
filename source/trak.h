#ifndef  XIAOC_MP4_TRAKBOX_H
#define  XIAOC_MP4_TRAKBOX_H
//作者  蔡砚刚  20171227  xiaoc@pku.edu.cn
#include "box.h"

class TRAKBOX : public BOX
{
public:
	int        subsubboxConter;//计数MOOV中的subbox个数
public:
	TRAKBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif