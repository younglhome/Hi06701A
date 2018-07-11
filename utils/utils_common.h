#ifndef UTILS_COMMON_H
#define UTILS_CPMMON_H
#ifdef __cplusplus
extern "C"{
#endif
#include <stdint.h>
#include <pthread.h>

void localtime_mp4name_get(char* dir, char* filename);
//��ȡosdʱ����ַ���
void localtime_string_get(char* tstr);
//ִ�п���ָ̨��
int exec_cmd(const char *cmd);		
//��ȡϵͳʣ���ڴ�
unsigned long get_system_mem_freeKb();		
//��ȡĿ¼ʣ��洢�ռ�
unsigned long long get_system_tf_freeKb(char* dir);
//��ȡϵͳ������ʱ�䵽���ڵ�tick��
int64_t get_tick_count();
//����4Mջ��С���߳�
int pthread_create_4m(pthread_t *pt_id, void *(*proc)(void *), void* arg);

#ifdef __cplusplus
}
#endif
#endif
