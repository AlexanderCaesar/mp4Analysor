#ifndef  XIAOC_MP4_STCOBOX_H
#define  XIAOC_MP4_STCOBOX_H
//����  �����  20180106  xiaoc@pku.edu.cn
#include "box.h"

class STCOBOX : public FullBox
{
public:

	unsigned  int   entry_count; //chunk����
    unsigned  int   chunk_offset;//ÿ��chunk��ƫ��ֵ,���ļ��׵�ַ��ʼƫ��

public:
	STCOBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif