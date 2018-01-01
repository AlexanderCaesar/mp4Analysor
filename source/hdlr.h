#ifndef  XIAOC_MP4_HDLRBOX_H
#define  XIAOC_MP4_HDLRBOX_H
//����  �����  20171227  xiaoc@pku.edu.cn
#include "box.h"

#define MAX_HDLRBOX_NAME_CHARS 256

class HDLRBOX : public FullBox
{
public:

	unsigned      int pre_defined;       //�ֶ�ռ32λ Ӧ��Ϊ0
	/*   vide  Video track
         soun  Audio track 
         hint  Hint track
    */
	Mp4xType          handler_type;      //�ֶ�ռ32λ ָʾ��ǰ���������
	unsigned      int reserved[3];       //�����ֶ�ռ32*3λ Ӧ��ȫΪ0
	unsigned     char name[MAX_HDLRBOX_NAME_CHARS];
	
public:
	HDLRBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif