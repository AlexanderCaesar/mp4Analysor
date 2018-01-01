#ifndef  XIAOC_MP4_SMHDBOX_H
#define  XIAOC_MP4_SMHDBOX_H
//����  �����  20171229  xiaoc@pku.edu.cn
#include "box.h"

class SMHDBOX : public FullBox
{
public:

	/*
	balance is a fixed - point 8.8 number that places mono audio tracks in a stereo space; 0 is center(the
	normal value); full left is - 1.0 and full right is 1.0.
	*/
	unsigned int  balance; //ռ16λ ǰ8λ��ʾ����λ����8λ��ʾС��λ һ��Ϊ typically 0.0  -1.0 ȫ��������  1.0 ȫ��Ϊ������
	unsigned int  reserved;//�����ֶ� 16λ
	
public:
	SMHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif