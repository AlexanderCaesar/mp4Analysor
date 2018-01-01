#ifndef  XIAOC_FTYP_BOX_H
#define  XIAOC_FTYP_BOX_H
//����  �����  20171217  xiaoc@pku.edu.cn

#define MAX_COMPATIBLE_BRANDS_CHARS 256
class FTYPBOX
{
public:
	unsigned int  major_brand;  // 32λ is a brand identifier
	unsigned int  minor_version;// 32λ s an informative integer for the minor version of the major brand
	unsigned char compatible_brands[MAX_COMPATIBLE_BRANDS_CHARS]; //is a list, to the end of the box, of brands

	unsigned __int64 boxSize;  // Box������size

	unsigned __int64 read_bytes; //�Ѿ���ȡ���Լ���Ŀ

	static int count; //����FTYPBOX���ֵĸ���
public:
	FTYPBOX(unsigned __int64 boxSize_, unsigned __int64 read_bytes_);
	int anlysis();
};
#endif