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
extern int  g_warning;       //ͳ�ƾ������

void openMp4Files();    //��mp4����ļ�
void closeMp4Files();   //�ر�mp4����ļ�

void initMp4Info();     //��ʼ��MP4���õ�һЩ����info��Ϣ
void desroryMp4Info();     //��ʼ��MP4���õ�һЩ����info��Ϣ


union Mp4xType
{
	unsigned char c_type[4]; // Box���� 32λ
	unsigned int  i_type;
};

struct MP4ELST
{
	unsigned __int64 segment_duration;//��ǰƬ�εĲ���ʱ��
	__int64          media_time;      //��ǰƬ�β��ŵ���ʼʱ�� ���ֵΪ-1����ʾ�ǿ�edit��һ��track�����һ��edit����Ϊ�ա�
};

struct MP4STTS
{
	unsigned     int sample_count;//��ǰƬ�ε�֡��
	unsigned     int sample_delta;//��ǰƬ�εĲ���ʱ��
};
struct MP4CTTS
{
	unsigned     int sample_count;//��ǰƬ�ε�֡��
	unsigned     int sample_offset;//��ǰƬ�ε���ʾʱ�����
};
struct MP4TimeStamp
{
	int              pts;//��ʾʱ���
	int              dts;//����ʱ���
	int              b_idr;//�Ƿ�ؼ���
	int              size;//�Ƿ�ؼ���
};

struct MP4STSS
{
	unsigned     int sample_number;//�������λ�� �ؼ�֡λ��
};

struct MP4SDTP
{
	unsigned int   reserved; //�����ֶ�ռ2λ
	unsigned int   sample_depends_on;//ռ2λ ��ǰ֡�ο�����֡���� 0����ȷ�� 1 �ο���I֡ 2 �ο�I֡ 3�����ֶ�
	unsigned int   sample_is_depended_on;//ռ2λ ��ǰ֡�α�������֡���� 0����ȷ�� 1 ��������֡�ο� 2 ������֡�ο� 3�����ֶ�
	unsigned int   sample_has_redundancy; //ռ2λ �Ƿ�������������� 0����ȷ�� 1 ӵ�� 2 ��ӵ�� 3��ȷ��
};

struct MP4STSC
{
	unsigned     int first_chunk;//��ǰ��ͬchunk��ĵ�һ��chunk��� ��1��ʼ���� 32λ
	unsigned     int samples_per_chunk;//ÿ��chunk��sample���� 32λ
	unsigned     int sample_description_index;//ֻ��stsd box����ֹһ����������λ�� 32λ
};

struct MP4STSZ
{
	unsigned     int entry_size;   //ÿ��sample�Ĵ�С
};

struct MP4STCO
{
	unsigned  int   chunk_offset;//ÿ��chunk��ƫ��ֵ
	unsigned  int   samples_per_chunk;//ÿ��chunk��ƫ��ֵ

};


#define  MAX_TRACKINFOR_NUM 128
struct TrackInfor
{
	Mp4xType          handler_type;      //�ֶ�ռ32λ ָʾ��ǰ���������
	unsigned      int track_ID;          //audio��track_ID
	unsigned      int timescale;         //һ���ӷָ��ķ��� �磺1000 ��ʾ 1���ӷ�Ϊ1000��
	unsigned      int elst_entry_count; //��ʾý��Ƭ����Ŀ
	MP4ELST*          elst;

	unsigned      int sum_sample_count; //��ʾ��ǰ�����֡��
	MP4TimeStamp*     timeStamp;//ÿһ֡��ʱ�����Ϣ

	MP4STTS*          stts;
	unsigned      int stts_entry_count; //��ʾý��Ƭ����Ŀ

	MP4CTTS*          ctts;
	unsigned      int ctts_entry_count; //��ʾý��Ƭ����Ŀ

	MP4STSS*          stss;
	unsigned      int stss_entry_count; //��ʾ�ؼ�֡��Ŀ

	MP4SDTP*          sdtp;//ÿ֡�Ĳο���Ϣ

	MP4STSC*          stsc;
	unsigned      int stsc_entry_count; //��ʾchunk����Ϣ

	MP4STSZ*          stsz;//ÿ֡��С

	MP4STCO*          stco;//ÿ��chunk��offset
	unsigned      int stco_entry_count; //��ʾchunk����

	
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