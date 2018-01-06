#ifndef  XIAOC_MP4_STTSBOX_H
#define  XIAOC_MP4_STTSBOX_H
//作者  蔡砚刚  20180103  xiaoc@pku.edu.cn
#include "box.h"

class STTSBOX : public FullBox
{
public:


	unsigned     int entry_count; //表示媒体片段数目

	unsigned     int sample_count;//当前片段的帧数
	unsigned     int sample_delta;//当前片段的播放时长

public:
	STTSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif