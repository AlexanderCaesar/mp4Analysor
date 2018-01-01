#ifndef  XIAOC_MP4_URLBOX_H
#define  XIAOC_MP4_URLBOX_H
//×÷Õß  ²ÌÑâ¸Õ  20171229  xiaoc@pku.edu.cn
#include "box.h"

#define MAX_LOCATION_CHARS 256
class URLBOX : public FullBox
{
public:

	unsigned char location[MAX_LOCATION_CHARS];
	
public:
	URLBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif