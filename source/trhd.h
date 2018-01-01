#ifndef  XIAOC_MP4_TRHDBOX_H
#define  XIAOC_MP4_TRHDBOX_H
//����  �����  20171227  xiaoc@pku.edu.cn
#include "box.h"

class TRHDBOX : public FullBox
{
public:

	unsigned __int64  creation_time;     //����ʱ��
	unsigned __int64  modification_time; //�޸�ʱ��
	unsigned      int track_ID;
	unsigned      int reserved0;         //�����ֶ�ռ32*2λ
	unsigned __int64  duration;          //����ʱ�� 

	unsigned      int reserved2[2];//�����ֶ�ռ32*2λ Ӧ��ȫΪ0

	unsigned      int layer;       //ռ16λ ��Ƶ�㲥�Ų�˳�� ���Խ��Խ������
	unsigned      int alternate_group ;//ռ16λ track ������Ϣ
	unsigned      int volume;          //ռ16λ ǰ8λ��ʾ����λ����8λ��ʾС��λ һ��Ϊ typically 1.0 (��Ƶ������� �������Ϊ0)  
	unsigned      int reserved1;//�����ֶ�ռ16λ Ӧ��ȫΪ0
	unsigned      int matrix[9];   //�ֶ�ռ32*9λ Ӧ��Ϊ { 0x00010000, 0, 0, 0, 0x00010000, 0, 0, 0, 0x40000000 };
	// unity matrix
	unsigned      int width;       //ռ32λ ��Ƶ��� ǰ16λ��ʾ����λ����16λ��ʾС��λ 
	unsigned      int height;      //ռ32λ ��Ƶ�߶� ǰ16λ��ʾ����λ����16λ��ʾС��λ 
public:
	TRHDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif