#ifndef HZK_BITMAP_H
#define HZK_BITMAP_H

#pragma pack(1)
typedef struct {
	unsigned short 	bfType; 	  //λͼ�ļ������ͣ�����ΪBM 
	unsigned int	bfSize; 	  //�ļ���С�����ֽ�Ϊ��λ
	unsigned short 	bfReserved1; //λͼ�ļ������֣�����Ϊ0 
	unsigned short 	bfReserved2; //λͼ�ļ������֣�����Ϊ0 
	unsigned int	bfOffBits;  //λͼ�ļ�ͷ�����ݵ�ƫ���������ֽ�Ϊ��λ
} BMPFILEHEADER_T;

typedef struct{
	unsigned int 	biSize;						//�ýṹ��С���ֽ�Ϊ��λ
	unsigned int  	biWidth;					   //ͼ�ο��������Ϊ��λ
	unsigned int  	biHeight; 					//ͼ�θ߶�������Ϊ��λ
	unsigned short 	biPlanes;			   //Ŀ���豸�ļ��𣬱���Ϊ1 
	unsigned short  biBitCount;			   //��ɫ��ȣ�ÿ����������Ҫ��λ��
	unsigned int  	biCompression;		 //λͼ��ѹ������
	unsigned int  	biSizeImage;				//λͼ�Ĵ�С�����ֽ�Ϊ��λ
	unsigned int  	biXPelsPerMeter;		 //λͼˮƽ�ֱ��ʣ�ÿ��������
	unsigned int  	biYPelsPerMeter;		 //λͼ��ֱ�ֱ��ʣ�ÿ��������
	unsigned int  	biClrUsed;			//λͼʵ��ʹ�õ���ɫ���е���ɫ��
	unsigned int  	biClrImportant;		//λͼ��ʾ��������Ҫ����ɫ��
} BMPINFOHEADER_T;

typedef struct{
	unsigned char b;
	unsigned char g;
	unsigned char r;
}RGB_T;
#pragma pack()

typedef enum{
	HZK_FONT_16 = 16,
	HZK_FONT_24 = 24,
	HZK_FONT_32 = 32,
}HZK_FONT_SIZE_E;

typedef struct{
	int width;
	int height;
	RGB_T* pRGB;
}BITMAP_INFO_T;

//public
int hzk_bitmap_create(char* str, int font, unsigned char fcolor, unsigned char bcolor, BITMAP_INFO_T* info);
int hzk_bitmap_destory(BITMAP_INFO_T info);
int hzk_bitmap_save(char* name, BITMAP_INFO_T info);

//private

#endif
