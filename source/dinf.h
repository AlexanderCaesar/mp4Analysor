#ifndef  XIAOC_MP4_DINFBOX_H
#define  XIAOC_MP4_DINFBOX_H
//����  �����  20171229  xiaoc@pku.edu.cn
#include "box.h"

class DINFBOX: public BOX //Data Information Box
{
public:
	int        subsubboxConter;//����EDTSBOX�е�subbox����
public:
	DINFBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif