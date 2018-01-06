#ifndef  XIAOC_MP4_METABOX_H
#define  XIAOC_MP4_METABOX_H
//作者  蔡砚刚  20180106  xiaoc@pku.edu.cn
#include "box.h"

class METABOX : public BOX
{
public:
	int        subsubboxConter;//计数EDTSBOX中的subbox个数
public:
	METABOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif