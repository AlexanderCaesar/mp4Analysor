#ifndef  XIAOC_MP4_CTTSBOX_H
#define  XIAOC_MP4_CTTSBOX_H
//����  �����  20180104  xiaoc@pku.edu.cn
#include "box.h"

class CTTSBOX : public FullBox
{
public:


	unsigned     int entry_count; //��ʾý��Ƭ����Ŀ

	unsigned     int sample_count;//��ǰƬ�ε�֡��
	unsigned     int sample_offset;//��ǰƬ�ε���ʾʱ�����

public:
	CTTSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif