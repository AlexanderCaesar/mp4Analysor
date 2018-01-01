#ifndef  XIAOC_MP4_MDHDBOX_H
#define  XIAOC_MP4_MDHDBOX_H
//作者  蔡砚刚  20171227  xiaoc@pku.edu.cn
#include "box.h"

class MDHDBOX : public FullBox
{
public:

	unsigned __int64  creation_time;     //创建时间
	unsigned __int64  modification_time; //修改时间
	unsigned      int timescale;         //一秒钟分隔的份数 如：1000 表示 1秒钟分为1000份
	unsigned __int64  duration;          //播放时长 

	unsigned     char language[3];       //共16位 剔除第一位 然后每五组一个字符 + 0xA0
	unsigned      int pre_defined;       //字段占16位 应该为0
public:
	MDHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif