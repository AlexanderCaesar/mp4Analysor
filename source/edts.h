#ifndef  XIAOC_MP4_EDTSBOX_H
#define  XIAOC_MP4_EDTSBOX_H
//����  �����  20171227  xiaoc@pku.edu.cn
#include "box.h"

class EDTSBOX : public BOX
{
public:
	int        subsubboxConter;//����EDTSBOX�е�subbox����
public:
	EDTSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif