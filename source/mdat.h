#ifndef  XIAOC_MP4_MDATBOX_H
#define  XIAOC_MP4_MDATBOX_H
//作者  蔡砚刚  20180106  xiaoc@pku.edu.cn
#include "box.h"

class MDATBOX : public BOX
{
public:
	int        subboxConter;//计数MOOV中的subbox个数

public:
	MDATBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif