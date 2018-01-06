//����  �����  20171214  xiaoc@pku.edu.cn
#include <stdlib.h>
#include "mp4file.h"

#pragma warning(disable: 4996) // POSIX setmode and fileno deprecated

FILE *g_mp4_file  = NULL;    //MP4 ԭʼ�ļ�
FILE *g_mp4_log = NULL;      //��ӡÿ��box����ϸ��Ϣ
FILE *g_mp4_video = NULL;    //�����Ƶ����Ϣ
FILE *g_mp4_audio = NULL;    //�����Ƶ����Ϣ
FILE *g_mp4_out = NULL;      //����޸ĺ��mp4��

char *mp4_file_name = NULL;  //mp4ԭʼ�ļ�            -i ���� �磺-i brazil-bq.mp4
char *mp4_log_name = NULL;   //��ӡÿ��box����ϸ��Ϣ  -t ���� �磺-t brazil-bq.log
char *mp4_video_name = NULL; //�����Ƶ����Ϣ         -v ���� �磺-v brazil-bq.264 
char *mp4_audio_name = NULL; //�����Ƶ����Ϣ         -a ���� �磺-a brazil-bq.aac 
char *mp4_out_name = NULL; //����޸ĺ��mp4��      -o ���� �磺-o brazil-new.mp4

int  g_errors        = 0;    //ͳ�ƴ������
int  g_warning       = 0;    //ͳ�ƾ������

Mp4Info mp4Info;

void initMp4Info()
{
	mp4Info.timescale= 1;
	mp4Info.current_track_ID = 0;
	mp4Info.audio_track_ID = 0;   //audio��track_ID
	mp4Info.video_track_ID = 0;   //video��track_ID

	for (int i = 0; i < MAX_TRACKINFOR_NUM; i++)
	{
		mp4Info.trackInfor[i].track_ID = 0;
		mp4Info.trackInfor[i].timescale = 0;
		mp4Info.trackInfor[i].elst_entry_count = 0;
		mp4Info.trackInfor[i].elst = NULL;
		mp4Info.trackInfor[i].stts_entry_count = 0;
		mp4Info.trackInfor[i].sum_sample_count = 0;
		mp4Info.trackInfor[i].stts = NULL;
		mp4Info.trackInfor[i].ctts_entry_count = 0;
		mp4Info.trackInfor[i].ctts = NULL;
		mp4Info.trackInfor[i].timeStamp = NULL;
		mp4Info.trackInfor[i].stss_entry_count = 0;
		mp4Info.trackInfor[i].stss = NULL;
		mp4Info.trackInfor[i].sdtp = NULL;
		mp4Info.trackInfor[i].stsc_entry_count = 0;
		mp4Info.trackInfor[i].stsc = NULL;
		mp4Info.trackInfor[i].stsz = NULL;
		mp4Info.trackInfor[i].stco_entry_count = 0;
		mp4Info.trackInfor[i].stco = NULL;

	}
}

void desroryMp4Info()
{
	for (int i = 0; i < MAX_TRACKINFOR_NUM; i++)
	{
		if (mp4Info.trackInfor[i].elst)
		{
			free(mp4Info.trackInfor[i].elst);
			mp4Info.trackInfor[i].elst = NULL;
		}

		if (mp4Info.trackInfor[i].stts)
		{
			free(mp4Info.trackInfor[i].stts);
			mp4Info.trackInfor[i].stts = NULL;
		}

		if (mp4Info.trackInfor[i].ctts)
		{
			free(mp4Info.trackInfor[i].ctts);
			mp4Info.trackInfor[i].ctts = NULL;
		}

		if (mp4Info.trackInfor[i].timeStamp)
		{
			free(mp4Info.trackInfor[i].timeStamp);
			mp4Info.trackInfor[i].timeStamp = NULL;
		}

		if (mp4Info.trackInfor[i].stss)
		{
			free(mp4Info.trackInfor[i].stss);
			mp4Info.trackInfor[i].stss = NULL;
		}

		if (mp4Info.trackInfor[i].sdtp)
		{
			free(mp4Info.trackInfor[i].sdtp);
			mp4Info.trackInfor[i].sdtp = NULL;
		}

		if (mp4Info.trackInfor[i].stsc)
		{
			free(mp4Info.trackInfor[i].stsc);
			mp4Info.trackInfor[i].stsc = NULL;
		}
		if (mp4Info.trackInfor[i].stsz)
		{
			free(mp4Info.trackInfor[i].stsz);
			mp4Info.trackInfor[i].stsz = NULL;
		}

		if (mp4Info.trackInfor[i].stco)
		{
			free(mp4Info.trackInfor[i].stco);
			mp4Info.trackInfor[i].stco = NULL;
		}
	}
}
//��mp4����ļ�
void openMp4Files()    
{
	if (mp4_file_name)
    {
        g_mp4_file = fopen(mp4_file_name,"rb");
        if(!g_mp4_file)
        {
            printf("���ļ�ʧ��: %s\n",mp4_file_name);
            system("pause");
            exit(0);
        }
    }
    if(mp4_log_name)
    {
        g_mp4_log = fopen(mp4_log_name,"w");
        if(!g_mp4_log)
        {
            printf("���ļ�ʧ��: %s\n",mp4_log_name);
            system("pause");
            exit(0);
        }
    }

    if(mp4_video_name)
    {
        g_mp4_video = fopen(mp4_video_name,"wb");
        if(!g_mp4_video)
        {
            printf("���ļ�ʧ��: %s\n",mp4_video_name);
            system("pause");
            exit(0);
        }
    }

    if(mp4_audio_name)
    {
        g_mp4_audio = fopen(mp4_audio_name,"wb");
        if(!g_mp4_audio)
        {
            printf("���ļ�ʧ��: %s\n",mp4_audio_name);
            system("pause");
            exit(0);
        }
    }

    if(mp4_out_name)
    {
        g_mp4_out = fopen(mp4_out_name,"wb");
        if(!g_mp4_out)
        {
            printf("���ļ�ʧ��: %s\n",mp4_out_name);
            system("pause");
            exit(0);
        }
    }
}

//�ر�mp4����ļ�
void closeMp4Files()
{
    if(g_mp4_file)
        fclose(g_mp4_file);
    if(g_mp4_log)
        fclose(g_mp4_log);
    if(g_mp4_video)
        fclose(g_mp4_video);
    if(g_mp4_audio)
        fclose(g_mp4_audio);
    if(g_mp4_out)
        fclose(g_mp4_out);
}