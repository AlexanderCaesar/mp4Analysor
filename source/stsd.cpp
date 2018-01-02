//����  �����  20171229  xiaoc@pku.edu.cn
#include "mp4file.h"
#include "stsd.h"
#pragma warning(disable:4996)

STSDBOX::STSDBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{
	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ

	subsubboxConter = 0;
}
int STSDBOX::anlysis()
{
	subsubboxConter = 0;   //��ʼ��Ϊ0
	fprintf(g_mp4_log, "\n++++++++++++++++++STSDBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");
	unsigned char data[128];
	int ret = fread(&version, 1, 1, g_mp4_file);

	if (ret !=1)
	{
		fprintf(g_mp4_log, "����:BOX version����1�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "version          :    %d\n", version);

	ret = fread(data, 1, 3, g_mp4_file);
	if (ret !=3)
	{
		fprintf(g_mp4_log, "����:flag����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	flag = (((unsigned int)data[0]) << 16) + (((unsigned int)data[1]) << 8) + (((unsigned int)data[2]) << 0);
	fprintf(g_mp4_log, "flag             :    %u\n", flag);

	read_bytes += 4;

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:entry_count����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	entry_count = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	read_bytes += 4;
	fprintf(g_mp4_log, "entry_count      :    %u ���������\n", entry_count);
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n++++++-------stsd subbox %5d--------++++++\n", subsubboxConter);
		if (box.readHeader() < 0)
			break;
		switch (mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.i_type)
		{
		case HANDLETYPE_VIDE:
			fprintf(g_mp4_log, "BOX    ����:    ������Ƶ��Ϣ\n");
			{
				VisualSampleEntryBOX vbox(box.boxSize, box.read_bytes);
				ret = vbox.anlysis();
				read_bytes += vbox.read_bytes;
			}
			break;

		case HANDLETYPE_SOUN:
			fprintf(g_mp4_log, "BOX    ����:    ������Ƶ��Ϣ\n");
			{
				AudioSampleEntryBOX abox(box.boxSize, box.read_bytes);
				ret = abox.anlysis();
				read_bytes += abox.read_bytes;
			}
			break;

		default:
			if (box.anlysis() < 0)//����box
				break;
			//ret = fseek(g_mp4_file, (long)(boxSize - read_bytes), SEEK_CUR);
			fprintf(g_mp4_log, "����:Handle type ����δ����������ӵ�������type %d %c%c%c%c error %d\n", mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.i_type, mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.c_type[0], mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.c_type[1], mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.c_type[2], mp4Info.trackInfor[mp4Info.current_track_ID].handler_type.c_type[3], g_errors); g_errors++;
			if (ret < 0)
			{
				fprintf(g_mp4_log, "����:�ļ���������error %d\n", g_errors); g_errors++;
			}
			read_bytes += box.boxSize;
			break;
		}


		subsubboxConter++;   //����Tag����    

		if (read_bytes == boxSize)
		{
			break;
		}
		else if (read_bytes > boxSize)
		{
			fprintf(g_mp4_log, "����:STSDBOX����box����(%d)����STSDBOX����(%d)������error %d\n", read_bytes, boxSize, g_errors); g_errors++;
			break;
		}
	}
	//STSDBOX��Ҫ��Ϣ
	fprintf(g_mp4_log, "\nstsd sub box ����:    %d\n", subsubboxConter);
	return 0;
}

VisualSampleEntryBOX::VisualSampleEntryBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{

	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ
}
int VisualSampleEntryBOX::anlysis()
{
	unsigned char data[12];

	fprintf(g_mp4_log, "\n++++++++++++++++++VisualSampleEntryBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");

	int ret = fread(data, 1, 6, g_mp4_file);
	if (ret !=6)
	{
		fprintf(g_mp4_log, "����:reserved0����6�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved0 = (((unsigned __int64)data[0]) << 40) + (((unsigned __int64)data[1]) << 32) + (((unsigned __int64)data[2]) << 24) + (((unsigned __int64)data[3]) << 16) + (((unsigned __int64)data[4]) << 8) + (((unsigned __int64)data[5]) << 0);
	if (reserved0 != 0)
	{
		fprintf(g_mp4_log, "����:reserved0 Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
	}
	fprintf(g_mp4_log, "reserved0          :    %lld \n", reserved0);
	read_bytes += 6;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:data_reference_index����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	data_reference_index = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "data_reference_index:    %d \n", data_reference_index);
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:pre_defined����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	pre_defined = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "pre_defined      :    %d Ԥ�ȶ����ֶ�\n", pre_defined);
	if (pre_defined != 0)
	{
		fprintf(g_mp4_log, "����:pre_defined Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:reserved����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "reserved         :    %d �����ֶ�\n", reserved);
	if (reserved != 0)
	{
		fprintf(g_mp4_log, "����:reserved Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	for (int i = 0; i < 3; i++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret !=4)
		{
			fprintf(g_mp4_log, "����:pre_defined����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		pre_defined1[i] = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "pre_defined1[%d]  :    %d Ԥ�ȶ����ֶ�\n", i, pre_defined1[i]);
		if (pre_defined1[i] != 0)
		{
			fprintf(g_mp4_log, "����:pre_defined1 Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
		}
		read_bytes += 4;
	}

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:width����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	width = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "width            :    %d \n", width);
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:height����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	height = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "height           :    %d \n", height);
	read_bytes += 2;

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:horizresolution����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	horizresolution = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "horizresolution  :    %d 0X%x\n", horizresolution, horizresolution);
	if (horizresolution != 0X480000)
	{
		fprintf(g_mp4_log, "����:horizresolution Ӧ��Ϊ0X480000 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 4;

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:vertresolution����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	vertresolution = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "vertresolution   :    %d 0X%x\n", vertresolution, vertresolution);
	if (vertresolution != 0X480000)
	{
		fprintf(g_mp4_log, "����:vertresolution Ӧ��Ϊ0X480000 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 4;

	ret = fread(data, 1, 4, g_mp4_file);
	if (ret !=4)
	{
		fprintf(g_mp4_log, "����:reserved2����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved2 = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "reserved2        :    %d \n", reserved2);
	if (reserved2 != 0)
	{
		fprintf(g_mp4_log, "����:reserved2 Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 4;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:frame_count����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	frame_count = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "frame_count      :    %d ÿ���������е���Ƶ֡�� Ĭ��Ϊ1\n", frame_count);
	read_bytes += 2;

	ret = fread(compressorname, 1, 32, g_mp4_file);
	if (ret !=32)
	{
		fprintf(g_mp4_log, "����:compressorname����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "compressorname   :    ");
	for (int i = 1; i <= compressorname[0]; i++)
	{
		fprintf(g_mp4_log, "%c", compressorname[i]);
	}
	fprintf(g_mp4_log, " �ַ����� %d\n", compressorname[0]);
	read_bytes += 32;
	
	ret = fread(data, 1, 2, g_mp4_file);
	if (ret !=2)
	{
		fprintf(g_mp4_log, "����:depth����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	depth = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "depth            :    %d 0x%x\n", depth,depth);
	if (depth != 0x18)
	{
		fprintf(g_mp4_log, "����:depth Ӧ��Ϊ0x18 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	ret = fread(&pre_defined2, 2, 1, g_mp4_file);
	if (ret !=1)
	{
		fprintf(g_mp4_log, "����:pre_defined2����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	fprintf(g_mp4_log, "pre_defined2     :    %d\n", pre_defined2);
	if (pre_defined2 != -1)
	{
		fprintf(g_mp4_log, "����:depth Ӧ��Ϊ0x18 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	long size = (long)(boxSize - read_bytes);

	int subsubboxConter = 0;   //��ʼ��Ϊ0
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n++++++-------avc1 subbox %5d-------+++++++\n", subsubboxConter);
		if (box.readHeader() < 0)
			break;

		if (box.anlysis() < 0)//����box
			break;

		read_bytes += box.boxSize;

		subsubboxConter++;   //����Tag����    

		if (read_bytes == boxSize)
		{
			break;
		}
		else if (read_bytes > boxSize)
		{
			fprintf(g_mp4_log, "����:AVC1����box����(%d)����AVC1����(%d)������error %d\n", read_bytes, boxSize, g_errors); g_errors++;
			break;
		}
	}
	//AVC1��Ҫ��Ϣ
	fprintf(g_mp4_log, "\navc1 sub box ����:    %d\n", subsubboxConter);

	return 0;
}

AudioSampleEntryBOX::AudioSampleEntryBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_)
{

	boxSize = boxSize_;  // Box������size

	read_bytes = read_bytes_; //�Ѿ���ȡ���Լ���Ŀ
}
int AudioSampleEntryBOX::anlysis()
{
	unsigned char data[12];

	fprintf(g_mp4_log, "\n++++++++++++++++++AudioSampleEntryBOX ��Ҫ��Ϣ+++++++++++++++++++++\n");

	int ret = fread(data, 1, 6, g_mp4_file);
	if (ret != 6)
	{
		fprintf(g_mp4_log, "����:reserved0����6�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved0 = (((unsigned __int64)data[0]) << 40) + (((unsigned __int64)data[1]) << 32) + (((unsigned __int64)data[2]) << 24) + (((unsigned __int64)data[3]) << 16) + (((unsigned __int64)data[4]) << 8) + (((unsigned __int64)data[5]) << 0);
	if (reserved0 != 0)
	{
		fprintf(g_mp4_log, "����:reserved0 Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
	}
	fprintf(g_mp4_log, "reserved0          :    %lld \n", reserved0);
	read_bytes += 6;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret != 2)
	{
		fprintf(g_mp4_log, "����:data_reference_index����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	data_reference_index = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "data_reference_index:    %d \n", data_reference_index);
	read_bytes += 2;

	for (int i = 0; i < 2; i++)
	{
		ret = fread(data, 1, 4, g_mp4_file);
		if (ret != 4)
		{
			fprintf(g_mp4_log, "����:reserved1����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
			return -1;
		}
		reserved1[i] = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
		fprintf(g_mp4_log, "reserved1[%d]     :    %d �����ֶ�\n", i, reserved1[i]);
		if (reserved1[i] != 0)
		{
			fprintf(g_mp4_log, "����:reserved1 Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
		}
		read_bytes += 4;
	}

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret != 2)
	{
		fprintf(g_mp4_log, "����:channelcount����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	channelcount = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "channelcount     :    %d ��Ƶ��������\n", channelcount);
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret != 2)
	{
		fprintf(g_mp4_log, "����:samplesize����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	samplesize = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "samplesize       :    %d ÿ��������ռ�õ�λ��\n", samplesize);
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret != 2)
	{
		fprintf(g_mp4_log, "����:pre_defined����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	pre_defined = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "pre_defined      :    %d Ԥ�ȶ����ֶ�\n", pre_defined);
	if (pre_defined != 0)
	{
		fprintf(g_mp4_log, "����:pre_defined Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;

	ret = fread(data, 1, 2, g_mp4_file);
	if (ret != 2)
	{
		fprintf(g_mp4_log, "����:reserved2����2�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	reserved2 = (((unsigned int)data[0]) << 8) + (((unsigned int)data[1]) << 0);
	fprintf(g_mp4_log, "reserved2        :    %d �����ֶ�\n", reserved2);
	if (reserved2 != 0)
	{
		fprintf(g_mp4_log, "����:reserved2 Ӧ��Ϊ0 error %d\n", g_errors); g_errors++;
	}
	read_bytes += 2;


	ret = fread(data, 1, 4, g_mp4_file);
	if (ret != 4)
	{
		fprintf(g_mp4_log, "����:samplerate����4�ֽ� �ļ����� error %d\n", g_errors); g_errors++;
		return -1;
	}
	samplerate = (((unsigned int)data[0]) << 24) + (((unsigned int)data[1]) << 16) + (((unsigned int)data[2]) << 8) + (((unsigned int)data[3]) << 0);
	fprintf(g_mp4_log, "samplerate       :    %u \n", samplerate>>16);
	if ((samplerate >> 16) != mp4Info.trackInfor[mp4Info.current_track_ID].timescale)
	{
		fprintf(g_mp4_log, "����:samplerate Ӧ��Ϊtimescale %d <<16 error %d\n", mp4Info.trackInfor[mp4Info.current_track_ID].timescale,g_errors); g_errors++;
	}
	read_bytes += 4;

	long size = (long)(boxSize - read_bytes);

	int subsubboxConter = 0;   //��ʼ��Ϊ0
	while (1)
	{
		BOX box;

		fprintf(g_mp4_log, "\n\n++++++-------avc1 subbox %5d-------+++++++\n", subsubboxConter);
		if (box.readHeader() < 0)
			break;

		if (box.anlysis() < 0)//����box
			break;

		read_bytes += box.boxSize;

		subsubboxConter++;   //����Tag����    

		if (read_bytes == boxSize)
		{
			break;
		}
		else if (read_bytes > boxSize)
		{
			fprintf(g_mp4_log, "����:AVC1����box����(%d)����AVC1����(%d)������error %d\n", read_bytes, boxSize, g_errors); g_errors++;
			break;
		}
	}
	//AVC1��Ҫ��Ϣ
	fprintf(g_mp4_log, "\navc1 sub box ����:    %d\n", subsubboxConter);

	return 0;
}