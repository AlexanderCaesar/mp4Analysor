#ifndef  XIAOC_MP4_SDTPBOX_H
#define  XIAOC_MP4_SDTPBOX_H
//����  �����  20180105  xiaoc@pku.edu.cn
#include "box.h"

class SDTPBOX : public FullBox
{
public:
	unsigned int   reserved; //�����ֶ�ռ2λ
	unsigned int   sample_depends_on;//ռ2λ ��ǰ֡�ο�����֡���� 0����ȷ�� 1 �ο���I֡ 2 �ο�I֡ 3�����ֶ�
	unsigned int   sample_is_depended_on;//ռ2λ ��ǰ֡�α�������֡���� 0����ȷ�� 1 ��������֡�ο� 2 ������֡�ο� 3�����ֶ�
	unsigned int   sample_has_redundancy; //ռ2λ �Ƿ�������������� 0����ȷ�� 1 ӵ�� 2 ��ӵ�� 3��ȷ��

public:
	SDTPBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif