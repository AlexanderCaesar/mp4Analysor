#ifndef  XIAOC_MP4_DREFBOX_H
#define  XIAOC_MP4_DREFBOX_H
//����  �����  20171229  xiaoc@pku.edu.cn
#include "box.h"

class DREFBOX: public FullBox //DataReferenceBox
{
public:
	int          subsubboxConter;//����EDTSBOX�е�subbox����
	unsigned int entry_count;    //���������
public:
	DREFBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif