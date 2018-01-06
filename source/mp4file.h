#ifndef  XIAOC_MP4_FILE_H
#define  XIAOC_MP4_FILE_H
//作者  蔡砚刚  20171214  xiaoc@pku.edu.cn

#include <stdio.h>
#include "mp4.h"


extern FILE *g_mp4_file;  //MP4 原始文件
extern FILE *g_mp4_log;   //打印每个box的详细信息
extern FILE *g_mp4_video; //输出视频流信息
extern FILE *g_mp4_audio; //输出音频流信息
extern FILE *g_mp4_out;   //输出修改后的mp4流

extern char *mp4_file_name;  //mp4 原始文件           -i 配置 如：-i brazil-bq.mp4
extern char *mp4_log_name;   //打印每个tag的详细信息  -t 配置 如：-t brazil-bq.log
extern char *mp4_video_name; //输出视频流信息         -v 配置 如：-v brazil-bq.264 
extern char *mp4_audio_name; //输出音频流信息         -a 配置 如：-a brazil-bq.aac 
extern char *mp4_out_name;   //输出修改后的mp4流      -o 配置 如：-o brazil-new.mp4 

extern int  g_errors;        //统计错误个数
extern int  g_warning;       //统计警告个数

void openMp4Files();    //打开mp4相关文件
void closeMp4Files();   //关闭mp4相关文件

void initMp4Info();     //初始化MP4常用的一些解析info信息
void desroryMp4Info();     //初始化MP4常用的一些解析info信息


union Mp4xType
{
	unsigned char c_type[4]; // Box类型 32位
	unsigned int  i_type;
};

struct MP4ELST
{
	unsigned __int64 segment_duration;//当前片段的播放时长
	__int64          media_time;      //当前片段播放的起始时间 如果值为-1，表示是空edit，一个track中最后一个edit不能为空。
};

struct MP4STTS
{
	unsigned     int sample_count;//当前片段的帧数
	unsigned     int sample_delta;//当前片段的播放时长
};
struct MP4CTTS
{
	unsigned     int sample_count;//当前片段的帧数
	unsigned     int sample_offset;//当前片段的显示时间戳差
};
struct MP4TimeStamp
{
	int              pts;//显示时间戳
	int              dts;//解码时间戳
	int              b_idr;//是否关键字
	int              size;//是否关键字
};

struct MP4STSS
{
	unsigned     int sample_number;//随机访问位置 关键帧位置
};

struct MP4SDTP
{
	unsigned int   reserved; //保留字段占2位
	unsigned int   sample_depends_on;//占2位 当前帧参考其它帧类型 0：不确定 1 参考非I帧 2 参考I帧 3保留字段
	unsigned int   sample_is_depended_on;//占2位 当前帧参被考其它帧类型 0：不确定 1 不被其它帧参考 2 被其它帧参考 3保留字段
	unsigned int   sample_has_redundancy; //占2位 是否有冗余编码数据 0：不确定 1 拥有 2 不拥有 3不确定
};

struct MP4STSC
{
	unsigned     int first_chunk;//当前相同chunk组的第一个chunk标号 从1开始计数 32位
	unsigned     int samples_per_chunk;//每个chunk的sample个数 32位
	unsigned     int sample_description_index;//只是stsd box（不止一个）的索引位置 32位
};

struct MP4STSZ
{
	unsigned     int entry_size;   //每个sample的大小
};

struct MP4STCO
{
	unsigned  int   chunk_offset;//每个chunk的偏移值
	unsigned  int   samples_per_chunk;//每个chunk的偏移值

};


#define  MAX_TRACKINFOR_NUM 128
struct TrackInfor
{
	Mp4xType          handler_type;      //字段占32位 指示当前轨道的类型
	unsigned      int track_ID;          //audio的track_ID
	unsigned      int timescale;         //一秒钟分隔的份数 如：1000 表示 1秒钟分为1000份
	unsigned      int elst_entry_count; //表示媒体片段数目
	MP4ELST*          elst;

	unsigned      int sum_sample_count; //表示当前轨道的帧数
	MP4TimeStamp*     timeStamp;//每一帧的时间戳信息

	MP4STTS*          stts;
	unsigned      int stts_entry_count; //表示媒体片段数目

	MP4CTTS*          ctts;
	unsigned      int ctts_entry_count; //表示媒体片段数目

	MP4STSS*          stss;
	unsigned      int stss_entry_count; //表示关键帧数目

	MP4SDTP*          sdtp;//每帧的参考信息

	MP4STSC*          stsc;
	unsigned      int stsc_entry_count; //表示chunk组信息

	MP4STSZ*          stsz;//每帧大小

	MP4STCO*          stco;//每个chunk的offset
	unsigned      int stco_entry_count; //表示chunk个数

	
};

enum HANDLETYPE
{
	HANDLETYPE_VIDE = 1701079414,
	HANDLETYPE_SOUN = 1853190003,
};

struct Mp4Info
{
	unsigned      int timescale;         //一秒钟分隔的份数 如：1000 表示 1秒钟分为1000份

	unsigned      int current_track_ID; //当前分析的track_ID

	unsigned      int audio_track_ID;   //audio的track_ID
	unsigned      int video_track_ID;   //video的track_ID
	TrackInfor        trackInfor[MAX_TRACKINFOR_NUM];

};

extern Mp4Info mp4Info;

#endif