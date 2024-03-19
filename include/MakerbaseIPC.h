/**
 * 进程间通信暂时不使用，此部分为消息队列，过程可能会出现阻塞现象，导致后面加载不起来
 * 
 */
#ifndef MAKERBASEIPC_H
#define MAKERBASEIPC_H

#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#define MKS_SEM_1_NAME    "mkssem1"       // 定义内部使用的信号量名称
#define MKS_SEM_2_NAME    "mkssem2"       // 定义内部使用的信号量名称

#define MKS_MSG_ID  318                     // 定义消息队列 id

struct commandBuf
{
    long mtype;
    char mtext[1024];
};

struct commandBuf cmd_to_rcv;

int msqid;
int ret;

sem_t *sem1 = NULL;         // 保存信号量地址
sem_t *sem2 = NULL;         // 保存信号量地址

ssize_t rcv_bytes;          // 保存从队列读取的字节数

#endif
