#ifndef  XIAOC_MP4_UDTABOX_H
#define  XIAOC_MP4_UDTABOX_H
//����  �����  20180106  xiaoc@pku.edu.cn
#include "box.h"

class UDTABOX : public BOX
{
public:
	int        subsubboxConter;//����MOOV�е�subbox����
public:
	UDTABOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif