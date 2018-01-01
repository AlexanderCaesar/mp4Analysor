#ifndef  XIAOC_MP4_WIDEBOX_H
#define  XIAOC_MP4_WIDEBOX_H
//×÷Õß  ²ÌÑâ¸Õ  20171229  xiaoc@pku.edu.cn
#include "box.h"

#define MAX_WIDE_CHARS 256

class WIDEBOX : public BOX
{
public:
	
	unsigned char data[MAX_WIDE_CHARS]; 

public:
	WIDEBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif