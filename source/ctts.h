#ifndef  XIAOC_MP4_CTTSBOX_H
#define  XIAOC_MP4_CTTSBOX_H
//作者  蔡砚刚  20180104  xiaoc@pku.edu.cn
#include "box.h"

class CTTSBOX : public FullBox
{
public:


	unsigned     int entry_count; //表示媒体片段数目

	unsigned     int sample_count;//当前片段的帧数
	unsigned     int sample_offset;//当前片段的显示时间戳差

public:
	CTTSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif