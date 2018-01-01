#ifndef  XIAOC_MP4_HDLRBOX_H
#define  XIAOC_MP4_HDLRBOX_H
//作者  蔡砚刚  20171227  xiaoc@pku.edu.cn
#include "box.h"

#define MAX_HDLRBOX_NAME_CHARS 256

class HDLRBOX : public FullBox
{
public:

	unsigned      int pre_defined;       //字段占32位 应该为0
	/*   vide  Video track
         soun  Audio track 
         hint  Hint track
    */
	Mp4xType          handler_type;      //字段占32位 指示当前轨道的类型
	unsigned      int reserved[3];       //保留字段占32*3位 应该全为0
	unsigned     char name[MAX_HDLRBOX_NAME_CHARS];
	
public:
	HDLRBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif