#ifndef  XIAOC_MP4_STSSBOX_H
#define  XIAOC_MP4_STSSBOX_H
//����  �����  20180105  xiaoc@pku.edu.cn
#include "box.h"

class STSSBOX : public FullBox
{
public:


	unsigned     int entry_count; //��ʾý��Ƭ����Ŀ

	unsigned     int sample_number;//�������λ�� �ؼ�֡λ��

public:
	STSSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif