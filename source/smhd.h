#ifndef  XIAOC_MP4_SMHDBOX_H
#define  XIAOC_MP4_SMHDBOX_H
//作者  蔡砚刚  20171229  xiaoc@pku.edu.cn
#include "box.h"

class SMHDBOX : public FullBox
{
public:

	/*
	balance is a fixed - point 8.8 number that places mono audio tracks in a stereo space; 0 is center(the
	normal value); full left is - 1.0 and full right is 1.0.
	*/
	unsigned int  balance; //占16位 前8位表示整数位，后8位表示小数位 一般为 typically 0.0  -1.0 全部左声道  1.0 全部为右声道
	unsigned int  reserved;//保留字段 16位
	
public:
	SMHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif