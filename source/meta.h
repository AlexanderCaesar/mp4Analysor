#ifndef  XIAOC_MP4_METABOX_H
#define  XIAOC_MP4_METABOX_H
//����  �����  20180106  xiaoc@pku.edu.cn
#include "box.h"

class METABOX : public BOX
{
public:
	int        subsubboxConter;//����EDTSBOX�е�subbox����
public:
	METABOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif