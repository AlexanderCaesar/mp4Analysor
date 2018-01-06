#ifndef  XIAOC_MP4_STSSBOX_H
#define  XIAOC_MP4_STSSBOX_H
//作者  蔡砚刚  20180105  xiaoc@pku.edu.cn
#include "box.h"

class STSSBOX : public FullBox
{
public:


	unsigned     int entry_count; //表示媒体片段数目

	unsigned     int sample_number;//随机访问位置 关键帧位置

public:
	STSSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif