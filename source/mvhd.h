#ifndef  XIAOC_MP4_MVHDBOX_H
#define  XIAOC_MP4_MVHDBOX_H
//作者  蔡砚刚  20171223  xiaoc@pku.edu.cn
#include "box.h"

class MVHDBOX : public FullBox
{
public:
	int        subboxConter;//计数MOOV中的subbox个数
	static int count; //计数FTYPBOX出现的个数

	unsigned __int64  creation_time;     //创建时间
	unsigned __int64  modification_time; //修改时间
	unsigned      int timescale;         //一秒钟分隔的份数 如：1000 表示 1秒钟分为1000份
	unsigned __int64  duration;          //播放时长 

	unsigned      int rate;        //占32位 播放速率 前16位表示整数位，后16位表示小数位 一般为 typically 1.0 
	unsigned      int volume;      //占16位 前8位表示整数位，后8位表示小数位 一般为 typically 1.0 
	unsigned      int reserved1   ;//保留字段占16位 应该全为0
	unsigned      int reserved2[2];//保留字段占32*2位 应该全为0
	unsigned      int matrix[9];   //字段占32*9位 应该为 { 0x00010000, 0, 0, 0, 0x00010000, 0, 0, 0, 0x40000000 };
	// Unity matrix
	unsigned      int pre_defined[6];//字段占32*6位 应该为0
	unsigned      int next_track_ID; //字段占32位 不应该为0
public:
	MVHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif