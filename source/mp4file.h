#ifndef  XIAOC_MP4_FILE_H
#define  XIAOC_MP4_FILE_H
//����  �����  20171214  xiaoc@pku.edu.cn

#include <stdio.h>
#include "mp4.h"


extern FILE *g_mp4_file;  //MP4 ԭʼ�ļ�
extern FILE *g_mp4_log;   //��ӡÿ��box����ϸ��Ϣ
extern FILE *g_mp4_video; //�����Ƶ����Ϣ
extern FILE *g_mp4_audio; //�����Ƶ����Ϣ
extern FILE *g_mp4_out;   //����޸ĺ��mp4��

extern char *mp4_file_name;  //mp4 ԭʼ�ļ�           -i ���� �磺-i brazil-bq.mp4
extern char *mp4_log_name;   //��ӡÿ��tag����ϸ��Ϣ  -t ���� �磺-t brazil-bq.log
extern char *mp4_video_name; //�����Ƶ����Ϣ         -v ���� �磺-v brazil-bq.264 
extern char *mp4_audio_name; //�����Ƶ����Ϣ         -a ���� �磺-a brazil-bq.aac 
extern char *mp4_out_name;   //����޸ĺ��mp4��      -o ���� �磺-o brazil-new.mp4 

extern int  g_errors;        //ͳ�ƴ������

void openMp4Files();    //��mp4����ļ�
void closeMp4Files();   //�ر�mp4����ļ�

void initMp4Info();     //��ʼ��MP4���õ�һЩ����info��Ϣ


union Mp4xType
{
	unsigned char c_type[4]; // Box���� 32λ
	unsigned int  i_type;
};

#define  MAX_TRACKINFOR_NUM 128
struct TrackInfor
{
	Mp4xType          handler_type;      //�ֶ�ռ32λ ָʾ��ǰ���������
	unsigned      int track_ID;          //audio��track_ID
};

enum HANDLETYPE
{
	HANDLETYPE_VIDE = 1701079414,
	HANDLETYPE_SOUN = 1853190003,
};

struct Mp4Info
{
	unsigned      int timescale;         //һ���ӷָ��ķ��� �磺1000 ��ʾ 1���ӷ�Ϊ1000��

	unsigned      int current_track_ID; //��ǰ������track_ID

	unsigned      int audio_track_ID;   //audio��track_ID
	unsigned      int video_track_ID;   //video��track_ID
	TrackInfor        trackInfor[MAX_TRACKINFOR_NUM];

};

extern Mp4Info mp4Info;

#endif