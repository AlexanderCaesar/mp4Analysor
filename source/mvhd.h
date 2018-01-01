#ifndef  XIAOC_MP4_MVHDBOX_H
#define  XIAOC_MP4_MVHDBOX_H
//����  �����  20171223  xiaoc@pku.edu.cn
#include "box.h"

class MVHDBOX : public FullBox
{
public:
	int        subboxConter;//����MOOV�е�subbox����
	static int count; //����FTYPBOX���ֵĸ���

	unsigned __int64  creation_time;     //����ʱ��
	unsigned __int64  modification_time; //�޸�ʱ��
	unsigned      int timescale;         //һ���ӷָ��ķ��� �磺1000 ��ʾ 1���ӷ�Ϊ1000��
	unsigned __int64  duration;          //����ʱ�� 

	unsigned      int rate;        //ռ32λ �������� ǰ16λ��ʾ����λ����16λ��ʾС��λ һ��Ϊ typically 1.0 
	unsigned      int volume;      //ռ16λ ǰ8λ��ʾ����λ����8λ��ʾС��λ һ��Ϊ typically 1.0 
	unsigned      int reserved1   ;//�����ֶ�ռ16λ Ӧ��ȫΪ0
	unsigned      int reserved2[2];//�����ֶ�ռ32*2λ Ӧ��ȫΪ0
	unsigned      int matrix[9];   //�ֶ�ռ32*9λ Ӧ��Ϊ { 0x00010000, 0, 0, 0, 0x00010000, 0, 0, 0, 0x40000000 };
	// Unity matrix
	unsigned      int pre_defined[6];//�ֶ�ռ32*6λ Ӧ��Ϊ0
	unsigned      int next_track_ID; //�ֶ�ռ32λ ��Ӧ��Ϊ0
public:
	MVHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif