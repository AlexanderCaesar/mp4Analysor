#ifndef  XIAOC_MP4_ELSTBOX_H
#define  XIAOC_MP4_ELSTBOX_H
//作者  蔡砚刚  20171227  xiaoc@pku.edu.cn
#include "box.h"

class ELSTBOX : public FullBox
{
public:

	unsigned     int entry_count; //表示媒体片段数目
	unsigned __int64 segment_duration;//当前片段的播放时长
    __int64          media_time;      //当前片段播放的起始时间 如果值为-1，表示是空edit，一个track中最后一个edit不能为空。

	/*specifies the relative rate at which to play the media corresponding
	to this edit segment.If this value is 0, then the edit is specifying a ‘dwell’ : 
	the media at media - time is presented for the segment - duration.
	Otherwise this field shall contain the value 1.*/
	int              media_rate_integer; //edit段的速率为0的话，edit段相当于一个”dwell”，即画面停止。画面会在media_time点上停止segment_duration时间。否则这个值始终为1。
	int              media_rate_fraction ;

public:
	ELSTBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif