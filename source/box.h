#ifndef  XIAOC_MP4_BOX_H
#define  XIAOC_MP4_BOX_H
//作者  蔡砚刚  20171214  xiaoc@pku.edu.cn

enum BOXTypes
{
	FTYP = 1887007846,
	MOOV = 1987014509,
	MVHD = 1684567661,
	TRAK = 1801548404,
	TRHD = 1684564852,
	EDTS = 1937007717,
	ELST = 1953721445,
	MDIA = 1634296941,
	MDHD = 1684563053,
	HDLR = 1919706216,
	MINF = 1718511981,
	VMHD = 1684565366,
	SMHD = 1684565363,
	DINF = 1718511972,
	DREF = 1717924452,
	URL  = 543978101,
	STBL = 1818391667,
	STSD  =1685288051,
	AVC1  =828601953,
	AVCC = 1130591841,
	ESDS = 1935962981,


	WIDE = 1701079415,
};

class BOX
{
public:
	unsigned int  size;         // Box长度 32位 如果为0 一直读取到文件末尾
	Mp4xType      type;         // Box类型 32位

	unsigned __int64 largesize;// Box长度 64位 如果size=1 有效

	unsigned __int64 boxSize;  // Box的最终size

	unsigned __int64 read_bytes; //已经读取的自己数目

public:
	BOX();
	int readHeader();
	virtual int anlysis();

};
class FullBox : public BOX
{
public:
	unsigned char version; //版本号 占用8bits
	unsigned int  flag;    //flag标志MAP 占用24bits
};
#endif
