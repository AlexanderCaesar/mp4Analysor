#ifndef  XIAOC_FTYP_BOX_H
#define  XIAOC_FTYP_BOX_H
//作者  蔡砚刚  20171217  xiaoc@pku.edu.cn

#define MAX_COMPATIBLE_BRANDS_CHARS 256
class FTYPBOX
{
public:
	unsigned int  major_brand;  // 32位 is a brand identifier
	unsigned int  minor_version;// 32位 s an informative integer for the minor version of the major brand
	unsigned char compatible_brands[MAX_COMPATIBLE_BRANDS_CHARS]; //is a list, to the end of the box, of brands

	unsigned __int64 boxSize;  // Box的最终size

	unsigned __int64 read_bytes; //已经读取的自己数目

	static int count; //计数FTYPBOX出现的个数
public:
	FTYPBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif