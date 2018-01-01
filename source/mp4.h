#ifndef XIAOC_MP4_H
#define XIAOC_MP4_H
//����  �����  20171214  xiaoc@pku.edu.cn

typedef struct mp4_param
{
    /* ������Ϣ
    -i ���� �磺-i brazil-bq.mp4
    -t ���� �磺-t brazil-bq.log
    -v ���� �磺-v brazil-bq.264
    -a ���� �磺-a brazil-bq.aac
	-o ���� �磺-o brazil-new.mp4
    **/
    int  b_video; //�Ƿ������Ƶ����Ϣ         -v ���� �磺-v brazil-bq.264 
    int  b_audio; //�Ƿ������Ƶ����Ϣ         -a ���� �磺-a brazil-bq.aac 
    int  b_n_mp4;  //�Ƿ�����µ�mp4�ļ�       -o ���� �磺-o brazil-new.mp4

}mp4_param;

extern mp4_param m_param; //���ò�����Ϣ

/* ����mp4��Ĭ�ϲ��� */
void mp4_param_default(mp4_param *param);

/* �������ò��� */
bool parse(mp4_param *param,int argc, char **argv);

#endif
