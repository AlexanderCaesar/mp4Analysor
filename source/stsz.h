#ifndef  XIAOC_MP4_STSZBOX_H
#define  XIAOC_MP4_STSZBOX_H
//作者  蔡砚刚  20180106  xiaoc@pku.edu.cn
#include "box.h"

class STSZBOX : public FullBox
{
public:

	unsigned     int sample_size;  //如果为0后续会有table 如果不为0 说明当前所有轨道的sample size都一致
	unsigned     int sample_count; //sample个数

	unsigned     int entry_size;   //每个sample的大小

public:
	STSZBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif