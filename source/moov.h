#ifndef  XIAOC_MP4_MOOVBOX_H
#define  XIAOC_MP4_MOOVBOX_H
//作者  蔡砚刚  20171217  xiaoc@pku.edu.cn
#include "box.h"

class MOOVBOX : public BOX
{
public:
	int        subboxConter;//计数MOOV中的subbox个数
	static int count; //计数FTYPBOX出现的个数
public:
	MOOVBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif