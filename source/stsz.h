#ifndef  XIAOC_MP4_STSZBOX_H
#define  XIAOC_MP4_STSZBOX_H
//����  �����  20180106  xiaoc@pku.edu.cn
#include "box.h"

class STSZBOX : public FullBox
{
public:

	unsigned     int sample_size;  //���Ϊ0��������table �����Ϊ0 ˵����ǰ���й����sample size��һ��
	unsigned     int sample_count; //sample����

	unsigned     int entry_size;   //ÿ��sample�Ĵ�С

public:
	STSZBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif