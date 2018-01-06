#ifndef  XIAOC_MP4_UDTABOX_H
#define  XIAOC_MP4_UDTABOX_H
//作者  蔡砚刚  20180106  xiaoc@pku.edu.cn
#include "box.h"

class UDTABOX : public BOX
{
public:
	int        subsubboxConter;//计数MOOV中的subbox个数
public:
	UDTABOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif