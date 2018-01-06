//作者  蔡砚刚  20171229  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "stbl.h"
#include <stdlib.h>
#include "common.h"
#pragma warning(disable:4996)

STBLBOX::STBLBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box的最终size

	read_bytes = read_bytes_; //已经读取的自己数目

	subsubboxConter = 0;
}
int STBLBOX::anlysis()
{
	subsubboxConter = 0;   //初始化为0
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n+++++--------stbl subbox %5d---------+++++\n", subsubboxConter);
		if (box.readHeader() < 0)
			break;

		if (box.anlysis() < 0)//分析box
			break;

		read_bytes += box.boxSize;

		subsubboxConter++;   //计数Tag个数    

		if (read_bytes == boxSize)
		{
			break;
		}
		else if (read_bytes > boxSize)
		{
			fprintf(g_mp4_log, "错误:STBLBOX的子box长度(%d)和与STBLBOX长度(%d)不符合error %d\n", read_bytes, boxSize, g_errors); g_errors++;
			break;
		}
	}
	fprintf(g_mp4_log, "\n\n+++++--------stbl 时间戳信息---------+++++\n");
	if (mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp == NULL)
	{
		mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp = (MP4TimeStamp*)malloc(mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count*sizeof(MP4TimeStamp));
		if (!mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp)
		{
			fprintf(g_mp4_log, "错误:MP4TimeStamp 申请内存出错 error %d\n", g_errors); g_errors++;
			return -1;
		}
	}

	if (mp4Info.trackInfor[mp4Info.current_track_ID].elst_entry_count == 1)
	{
		int curretn_pts = 0;
		int sample_count = 0;
		curretn_pts -= (int)mp4Info.trackInfor[mp4Info.current_track_ID].elst[0].media_time;
		for (unsigned int stts_entry = 0; stts_entry < mp4Info.trackInfor[mp4Info.current_track_ID].stts_entry_count; stts_entry++)
		{
			for (unsigned int count = 0; count < mp4Info.trackInfor[mp4Info.current_track_ID].stts[stts_entry].sample_count; count++)
			{
				mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[sample_count].pts =
					mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[sample_count].dts = curretn_pts;
				curretn_pts += mp4Info.trackInfor[mp4Info.current_track_ID].stts[stts_entry].sample_delta;
				mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[sample_count].b_idr = 0;
				mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[sample_count].size = (int)mp4Info.trackInfor[mp4Info.current_track_ID].stsz[sample_count].entry_size;
				sample_count++;
			} 
		}
		for (unsigned int stss_entry = 0; stss_entry < mp4Info.trackInfor[mp4Info.current_track_ID].stss_entry_count; stss_entry++)
		{
			int idr_index = mp4Info.trackInfor[mp4Info.current_track_ID].stss[stss_entry].sample_number - 1;
			mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[idr_index].b_idr = 1;
		}
		sample_count = 0;
		for (unsigned int ctts_entry = 0; ctts_entry < mp4Info.trackInfor[mp4Info.current_track_ID].ctts_entry_count; ctts_entry++)
		{
			for (unsigned int count = 0; count < mp4Info.trackInfor[mp4Info.current_track_ID].ctts[ctts_entry].sample_count; count++)
			{
				mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[sample_count].dts -=
					mp4Info.trackInfor[mp4Info.current_track_ID].ctts[ctts_entry].sample_offset;
				sample_count++;
			}
		}
		fprintf(g_mp4_log, "handletype %c%c%c%c\n", mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.c_type[0], mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.c_type[1], mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.c_type[2], mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.c_type[3]);
		fprintf(g_mp4_log, "sample num           pts                          dts             size     关键帧");
		if (mp4Info.trackInfor[mp4Info.current_track_ID].sdtp)
		{
			fprintf(g_mp4_log, "   参考其它帧关系        被参考关系        冗余数据");
		}
		fprintf(g_mp4_log, "\n");
		char rtimepts[128];
		char rtimedts[128];
		char idrchars[12];
		for (unsigned int i = 0; i < mp4Info.trackInfor[mp4Info.current_track_ID].sum_sample_count; i++)
		{
			printfTimeStamp((unsigned __int64)abs(mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[i].dts), rtimedts);
			printfTimeStamp((unsigned __int64)abs(mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[i].pts), rtimepts);
			if (mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[i].b_idr)
			{
				sprintf(idrchars, "1 IDR");
			}
			else
			{
				sprintf(idrchars, "0    ");
			}
			fprintf(g_mp4_log, "%5d       %5d (%s)         %5d (%s)    %5d     %s", i, mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[i].pts, rtimepts, mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[i].dts, rtimedts, mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[i].size, idrchars);
			if (mp4Info.trackInfor[mp4Info.current_track_ID].sdtp)
			{
				char depends_on[128];
				char is_depended_on[128];
				char has_redundancy[128];
				unsigned int sample_depends_on = mp4Info.trackInfor[mp4Info.current_track_ID].sdtp[i].sample_depends_on;
				unsigned int sample_is_depended_on = mp4Info.trackInfor[mp4Info.current_track_ID].sdtp[i].sample_is_depended_on;
				unsigned int sample_has_redundancy = mp4Info.trackInfor[mp4Info.current_track_ID].sdtp[i].sample_has_redundancy;
				switch (sample_depends_on)
				{
				case 0: sprintf(depends_on, "%d             ", sample_depends_on); break;
				case 1: sprintf(depends_on, "%d 参考非I帧   ", sample_depends_on); break;
				case 2: sprintf(depends_on, "%d 参考I帧     ", sample_depends_on); break;
				case 3: sprintf(depends_on, "%d 保留字段    ", sample_depends_on); break;
				default:
					sprintf(depends_on, "%d 保留字段    ", sample_depends_on); break;
					fprintf(g_mp4_log, "错误:sample_depends_on 未知 %d 文件有损坏 error %d\n", sample_depends_on, g_errors); g_errors++;
				}
				switch (sample_is_depended_on)
				{
				case 0: sprintf(is_depended_on, "%d             ", sample_is_depended_on); break;
				case 1: sprintf(is_depended_on, "%d 不被它帧参考", sample_is_depended_on); break;
				case 2: sprintf(is_depended_on, "%d 被它帧参考  ", sample_is_depended_on); break;
				case 3: sprintf(is_depended_on, "%d 保留字段    ", sample_is_depended_on); break;
				default:
					sprintf(is_depended_on, "%d 保留字段    ", sample_is_depended_on); break;
					fprintf(g_mp4_log, "错误:sample_is_depended_on 未知 %d 文件有损坏 error %d\n", sample_is_depended_on, g_errors); g_errors++;
				}
				switch (sample_has_redundancy)
				{
				case 0: sprintf(has_redundancy, "%d             ", sample_has_redundancy); break;
				case 1: sprintf(has_redundancy, "%d 拥有冗余信息", sample_has_redundancy); break;
				case 2: sprintf(has_redundancy, "%d 没有冗余信息", sample_has_redundancy); break;
				case 3: sprintf(has_redundancy, "%d 保留字段    ", sample_has_redundancy); break;
				default:
					sprintf(has_redundancy, "%d 保留字段    ", sample_has_redundancy); break;
					fprintf(g_mp4_log, "错误:sample_has_redundancy 未知 %d 文件有损坏 error %d\n", sample_has_redundancy, g_errors); g_errors++;
				}
				fprintf(g_mp4_log, "  %s         %s     %s", depends_on, is_depended_on, has_redundancy);
			}
			if (mp4Info.trackInfor[mp4Info.current_track_ID].timeStamp[i].pts < 0)
			{
				fprintf(g_mp4_log, "警告:该帧可能不会被播放详见elst warning %d", g_warning); g_warning++;
			}
			fprintf(g_mp4_log, "\n");
		}
	}
	else
	{
		fprintf(g_mp4_log, "错误:elst_entry_count %d 不为1 后续待支持 error %d\n", mp4Info.trackInfor[mp4Info.current_track_ID].elst_entry_count, g_errors); g_errors++;
	}

	//STBLBOX概要信息
	fprintf(g_mp4_log, "\nstbl sub box 个数:    %d\n", subsubboxConter);
	return 0;
}