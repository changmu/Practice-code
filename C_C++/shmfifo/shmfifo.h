#pragma once

#include "myLinux.h"
#include "ipc.h"

typedef struct shmfifo shmfifo_t;
typedef struct shmhead shmhead_t;

struct shmhead {
        unsigned blksize;
        unsigned blocks;        // 总块数
        unsigned rd_index;
        unsigned wr_index;
};

struct shmfifo {
        shmhead_t *p_shm;       // 共享内存头指针
        char *p_payload;        // 有效负载起始地址

        int shmid;              // 共享内存ID
        int sem_mutex;          // 互斥信号量
        int sem_full;           // 控制空槽数量
        int sem_empty;          // 控制产品数量
};

shmfifo_t *shmfifo_init(int key, int blksize, int blocks);
void shmfifo_put(shmfifo_t *fifo, const void *buf);
void shmfifo_get(shmfifo_t *fifo, char *buf);
void shmfifo_destroy(shmfifo_t *fifo);
