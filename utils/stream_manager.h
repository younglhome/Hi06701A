#ifndef STREAM_MANAGER_H
#define STREAM_MANAGER_H

#include "lock_utils.h"

typedef enum{
	SHM_STREAM_READ = 1,
	SHM_STREAM_WRITE,
	SHM_STREAM_WRITE_BLOCK,
}SHM_STREAM_MODE_E;

typedef struct
{
	int type;
	int key;
	unsigned int 		length;
	unsigned long long 	pts;
	char reserved[12];
}frame_info;

typedef struct 
{
	char			id[32];	//	�û���ʶ
	unsigned int	index;	//	��ǰ��дinfo_array�±�
	unsigned int	offset;	//	��ǰ���ݴ洢ƫ�� ֻ����дģʽ
	unsigned int	users;	//	���û���
}shm_user_t;

typedef struct 
{
	unsigned int	offset;		//	���ݴ洢ƫ��
	unsigned int	lenght;		//	���ݳ���
	frame_info		info;		//	����info
}shm_info_t;

typedef struct 
{
//private
	char*	user_array;			//	�û������ʼ��ַ
	char*	info_array;			//	��Ϣ�����ʼ��ַ
	char*	base_addr;			//	���ݳ�ʼ��ַ
	CSem	sem;
	char	name[20];			//	�����Ĺ����ļ���
	unsigned int index;			//	�û���userArray�е��±�
	unsigned int max_frames;	//	����֡����������Ҫ��frameArray����Ԫ�ظ���
	unsigned int size;
	SHM_STREAM_MODE_E mode;
}shm_stream_t;

//public
shm_stream_t* shm_stream_create(char* id, char* name, int users, int infos, int size, SHM_STREAM_MODE_E mode);
void shm_stream_destory(shm_stream_t* handle);

int shm_stream_put(shm_stream_t* handle, frame_info info, unsigned char* data, unsigned int length);
int shm_stream_get(shm_stream_t* handle, frame_info* info, unsigned char** data, unsigned int* length);
int shm_stream_post(shm_stream_t* handle);
int shm_stream_sync(shm_stream_t* handle);
int shm_stream_remains(shm_stream_t* handle);
int shm_stream_readers(shm_stream_t* handle);

//private
void* shm_stream_mmap(shm_stream_t* handle, char* name, unsigned int size);

#endif
