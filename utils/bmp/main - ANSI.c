#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define size 32
#define sum (size*size/8)

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

unsigned char* convert(char* HZ, int hz_count, RGB_T* pRGB);
void Snapshot(unsigned char* pData, int width, int height,  char * filename );

void dbc_to_sbc(char *bdest, char *bsrc)
{
    for(; *bsrc; ++bsrc)
    {
        if((*bsrc & 0xff) == 0x20)
        {
            *bdest++ = 0xA1;
            *bdest++ = 0xA1;
        }
        else if((*bsrc & 0xff) >= 0x21 && (*bsrc & 0xff) <= 0x7E)
        {
            *bdest++ = 0xA3;
            *bdest++ = *bsrc + 0x80;
        }
        else
        {
            if(*bsrc < 0){
				*bdest++ = *bsrc++;
				*bdest++ = *bsrc;
			}
            //*bdest++ = *bsrc;
        }
    }
    *bdest = 0;
}

void local_time_get(char* tstr)
{
	char *wday[]={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	time_t timep;
	struct tm *p;
	
	time(&timep);
	p=localtime(&timep);
	
	sprintf(tstr,"%d/%02d/%02d %s %02d:%02d:%02d", (1900+p->tm_year),(1+p->tm_mon), p->tm_mday
		, wday[p->tm_wday],p->tm_hour, p->tm_min, p->tm_sec);
}

void file_time_get(char* tstr)
{
	time_t timep;
	struct tm *p;
	
	time(&timep);
	p=localtime(&timep);
	
	sprintf(tstr,"%d-%02d-%02d %02d-%02d-%02d", (1900+p->tm_year),(1+p->tm_mon), p->tm_mday
		,p->tm_hour, p->tm_min, p->tm_sec);
}

int main(int arg0, char* argv[])
{
	if(arg0 > 1)
		printf("%d %s\n", arg0, argv[1]);

	char t_str[1024] = {0};
	while(1)
	{
		usleep(300*1000);

		//ǰ����ASIIC���� GB2312
		char h0[1024] = {0}; 

		local_time_get(h0);
		if(strcmp(t_str, h0) != 0)
		{
			printf("%s\n", h0);
			char hz[1024] = {0};
			dbc_to_sbc(hz, h0);
//			dbc_to_sbc(hz, argv[1]);

			int hz_count = strlen(hz)/2;				// ��Ϊȫ��ת������ȫ��
			RGB_T pRGB[size][size*hz_count];			// ����λͼ����

			memset( pRGB, 0, sizeof(pRGB) );			// ���ñ���Ϊ��ɫ
			convert(hz, hz_count, pRGB[0]);

			char filename[1024] = {0};
			file_time_get(filename);
			sprintf(filename, "%s.bmp", filename);
			printf("%s\n", filename);
			Snapshot((unsigned char*)pRGB, size*hz_count, size, filename);
			strcpy(t_str, h0);
		}
	}
	
	return 0;
}

unsigned char* convert(char* HZ, int hz_count, RGB_T* pRGB) 
{    
	char  hz[64];
	FILE* HZK = NULL;

	sprintf(hz , "HZK%d" , size);
	HZK = fopen(hz , "rb");
	if( HZK == NULL)  
		//fail to open file
	{  
		printf("Can't Open hzk%d\n" , size);  
		getchar(); 
		return NULL; 
	}

	int ii=0;
	for(ii=0; ii<hz_count; ii++)
	{
		unsigned char qh = HZ[ii*2+0] - 0xa0; //�������  
		unsigned char wh = HZ[ii*2+1] - 0xa0; //���λ��

		unsigned long offset = 0;
		unsigned char mat[sum][size/8] = {0} ; //mat[sum][2]

		offset = (94*(qh-1)+(wh-1))*sum; //�õ�ƫ��λ��
		fseek(HZK, offset, SEEK_SET); 
		fread(mat, sum , 1, HZK);

		int col=0;
		int row=0;
		//��ʾ 
		for(int i=0; i<size; i++) //16
		{ 
			for(int j=0; j<size/8; j++) //2
			{ 
				for(int k=0; k<8 ; k++) //8bits , 1 byte
				{ 
					if( mat[i][j] & (0x80>>k))
					{//����Ϊ1��λ����ʾ 
						//������ɫ
						pRGB[(size-1-row)*(size*hz_count) + col + ii*size].r = 0xff;//bmp�е������굹��
						pRGB[(size-1-row)*(size*hz_count) + col + ii*size].g = 0xff;
						pRGB[(size-1-row)*(size*hz_count) + col + ii*size].b = 0xff;
					} 
					else 
					{	//����ɫ
						pRGB[(size-1-row)*(size*hz_count) + col + ii*size].r = 0x00;//bmp�е������굹��
						pRGB[(size-1-row)*(size*hz_count) + col + ii*size].g = 0x00;
						pRGB[(size-1-row)*(size*hz_count) + col + ii*size].b = 0x00;
					} 
					col++;
				}
			}
			row++;
			col = 0;
		}//for
	}
	
	fclose(HZK); 
}

void Snapshot(unsigned char* pData, int width, int height,  char * filename )
{
	int bmpsize = width*height*3; // ÿ�����ص�3���ֽ�

	// λͼ��һ���֣��ļ���Ϣ
	BMPFILEHEADER_T bfh;
	bfh.bfType = 0x4d42;  //bm
	bfh.bfSize = bmpsize  // data size
		+ sizeof( BMPFILEHEADER_T ) // first section size
		+ sizeof( BMPINFOHEADER_T ) // second section size
		;
	bfh.bfReserved1 = 1; // reserved 
	bfh.bfReserved2 = 0; // reserved
	bfh.bfOffBits = bfh.bfSize - bmpsize;
	// λͼ�ڶ����֣�������Ϣ
	BMPINFOHEADER_T bih;
	bih.biSize = sizeof(BMPINFOHEADER_T);
	bih.biWidth = width;
	bih.biHeight = height;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = 0;
	bih.biSizeImage = bmpsize;
	bih.biXPelsPerMeter = 0;
	bih.biYPelsPerMeter = 0;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;       

	FILE * fp = fopen(filename,"wb");
	if( !fp ) return;

	fwrite( &bfh, 1, sizeof(BMPFILEHEADER_T), fp );
	fwrite( &bih, 1, sizeof(BMPINFOHEADER_T), fp );
	fwrite( pData, 1, bmpsize, fp );
	fclose( fp );
}


