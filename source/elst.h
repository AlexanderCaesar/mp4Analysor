#ifndef  XIAOC_MP4_ELSTBOX_H
#define  XIAOC_MP4_ELSTBOX_H
//����  �����  20171227  xiaoc@pku.edu.cn
#include "box.h"

class ELSTBOX : public FullBox
{
public:

	unsigned     int entry_count; //��ʾý��Ƭ����Ŀ
	unsigned __int64 segment_duration;//��ǰƬ�εĲ���ʱ��
    __int64          media_time;      //��ǰƬ�β��ŵ���ʼʱ�� ���ֵΪ-1����ʾ�ǿ�edit��һ��track�����һ��edit����Ϊ�ա�

	/*specifies the relative rate at which to play the media corresponding
	to this edit segment.If this value is 0, then the edit is specifying a ��dwell�� : 
	the media at media - time is presented for the segment - duration.
	Otherwise this field shall contain the value 1.*/
	int              media_rate_integer; //edit�ε�����Ϊ0�Ļ���edit���൱��һ����dwell����������ֹͣ���������media_time����ֹͣsegment_durationʱ�䡣�������ֵʼ��Ϊ1��
	int              media_rate_fraction ;

public:
	ELSTBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif