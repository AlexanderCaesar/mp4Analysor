#ifndef  XIAOC_MP4_MOOVBOX_H
#define  XIAOC_MP4_MOOVBOX_H
//����  �����  20171217  xiaoc@pku.edu.cn
#include "box.h"

class MOOVBOX : public BOX
{
public:
	int        subboxConter;//����MOOV�е�subbox����
	static int count; //����FTYPBOX���ֵĸ���
public:
	MOOVBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif