#ifndef  XIAOC_MP4_STTSBOX_H
#define  XIAOC_MP4_STTSBOX_H
//����  �����  20180103  xiaoc@pku.edu.cn
#include "box.h"

class STTSBOX : public FullBox
{
public:


	unsigned     int entry_count; //��ʾý��Ƭ����Ŀ

	unsigned     int sample_count;//��ǰƬ�ε�֡��
	unsigned     int sample_delta;//��ǰƬ�εĲ���ʱ��

public:
	STTSBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif