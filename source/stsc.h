#ifndef  XIAOC_MP4_STSCBOX_H
#define  XIAOC_MP4_STSCBOX_H
//����  �����  20180105  xiaoc@pku.edu.cn
#include "box.h"
/*����
first_chunk  samples_per_chunk  sample_description_index
 1             15                  1
 105           30                  1
 106           22                  1
 �����ʾ�ܹ�106��chunk
 ��Ϊ������ ��
 �ֱ���ÿ��chunk��15��sample ��ǰchunk��ӱ��Ϊ1��ʼһֱ�����Ϊ104
 �ֱ���ÿ��chunk��30��sample ��ǰchunk��ֻ��һ��chunk ���105
 �ֱ���ÿ��chunk��22��sample ��ǰchunk��ֻ��һ��chunk ���106
*/
class STSCBOX : public FullBox
{
public:
	unsigned     int entry_count; //��ʾý��Ƭ����Ŀ


	unsigned     int first_chunk;//��ǰ��ͬchunk��ĵ�һ��chunk��� ��1��ʼ���� 32λ
	unsigned     int samples_per_chunk;//ÿ��chunk��sample���� 32λ
	unsigned     int sample_description_index;//ֻ��stsd box����ֹһ����������λ�� 32λ

public:
	STSCBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif