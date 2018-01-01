#ifndef  XIAOC_MP4_MDHDBOX_H
#define  XIAOC_MP4_MDHDBOX_H
//����  �����  20171227  xiaoc@pku.edu.cn
#include "box.h"

class MDHDBOX : public FullBox
{
public:

	unsigned __int64  creation_time;     //����ʱ��
	unsigned __int64  modification_time; //�޸�ʱ��
	unsigned      int timescale;         //һ���ӷָ��ķ��� �磺1000 ��ʾ 1���ӷ�Ϊ1000��
	unsigned __int64  duration;          //����ʱ�� 

	unsigned     char language[3];       //��16λ �޳���һλ Ȼ��ÿ����һ���ַ� + 0xA0
	unsigned      int pre_defined;       //�ֶ�ռ16λ Ӧ��Ϊ0
public:
	MDHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif