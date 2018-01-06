#ifndef  XIAOC_MP4_OTHERBOX_H
#define  XIAOC_MP4_OTHERBOX_H
//×÷Õß  ²ÌÑâ¸Õ  20171229  xiaoc@pku.edu.cn
#include "box.h"

#define MAX_WIDE_CHARS 256

class OTHERBOX : public BOX
{
public:
	
	unsigned char data[MAX_WIDE_CHARS]; 

public:
	OTHERBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif