#ifndef  XIAOC_MP4_STCOBOX_H
#define  XIAOC_MP4_STCOBOX_H
//作者  蔡砚刚  20180106  xiaoc@pku.edu.cn
#include "box.h"

class STCOBOX : public FullBox
{
public:

	unsigned  int   entry_count; //chunk个数
    unsigned  int   chunk_offset;//每个chunk的偏移值,从文件首地址开始偏移

public:
	STCOBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif