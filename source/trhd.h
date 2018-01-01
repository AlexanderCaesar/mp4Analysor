#ifndef  XIAOC_MP4_TRHDBOX_H
#define  XIAOC_MP4_TRHDBOX_H
//作者  蔡砚刚  20171227  xiaoc@pku.edu.cn
#include "box.h"

class TRHDBOX : public FullBox
{
public:

	unsigned __int64  creation_time;     //创建时间
	unsigned __int64  modification_time; //修改时间
	unsigned      int track_ID;
	unsigned      int reserved0;         //保留字段占32*2位
	unsigned __int64  duration;          //播放时长 

	unsigned      int reserved2[2];//保留字段占32*2位 应该全为0

	unsigned      int layer;       //占16位 视频层播放层顺序 序号越低越靠顶层
	unsigned      int alternate_group ;//占16位 track 分组信息
	unsigned      int volume;          //占16位 前8位表示整数位，后8位表示小数位 一般为 typically 1.0 (音频轨道有用 其它轨道为0)  
	unsigned      int reserved1;//保留字段占16位 应该全为0
	unsigned      int matrix[9];   //字段占32*9位 应该为 { 0x00010000, 0, 0, 0, 0x00010000, 0, 0, 0, 0x40000000 };
	// unity matrix
	unsigned      int width;       //占32位 视频宽度 前16位表示整数位，后16位表示小数位 
	unsigned      int height;      //占32位 视频高度 前16位表示整数位，后16位表示小数位 
public:
	TRHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif