#ifndef  XIAOC_MP4_VMHDBOX_H
#define  XIAOC_MP4_VMHDBOX_H
//×÷Õß  ²ÌÑâ¸Õ  20171227  xiaoc@pku.edu.cn
#include "box.h"

class VMHDBOX : public FullBox
{
public:

	/*pecifies a composition mode for this video track, from the following enumerated set,
      which may be extended by derived specifications:
	  copy = 0 copy over the existing image
	*/
	unsigned int  graphicsmode; // 16Î»copy, see below template
	unsigned int  red;
	unsigned int  green;
	unsigned int  blue;
	
public:
	VMHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif