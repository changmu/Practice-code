#include <myLinux.h>

struct mt {
    int                 num;
    pthread_mutex_t     mutex;
    pthread_mutexattr_t mutexattr;
};

int main(int argc, char *argv[])
{
    int fd, i;
    struct mt *mm;
    pid_t pid;
    int err;

    fd = open("mt_test", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof (*mm));
    mm = mmap(NULL, sizeof (*mm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    memset(mm, 0, sizeof (*mm));

    /* 初始化互斥属性 */
    pthread_mutexattr_init(&mm->mutexattr);
    /* 设置属性为PTHREAD_PROCESS_SHARED共享 */
    pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);

    /* 初始化互斥锁 */
    pthread_mutex_init(&mm->mutex, &mm->mutexattr);

    if ((pid = fork()) == 0) {
        for (i = 0; i < 10; ++i) {
            pthread_mutex_lock(&mm->mutex);
            ++mm->num;
            printf("num++: %d\n", mm->num);
            pthread_mutex_unlock(&mm->mutex);
            sleep(1);
        }
    } else {
        for (i = 0; i < 10; ++i) {
            pthread_mutex_lock(&mm->mutex);
            mm->num += 2;
            printf("num += 2: %d\n", mm->num);
            pthread_mutex_unlock(&mm->mutex);
            sleep(1);
        }
        wait(NULL);

        err = pthread_mutex_destroy(&mm->mutex);
        if (err)
            printf("%s\n", strerror(err));
        err = pthread_mutexattr_destroy(&mm->mutexattr);
        if (err)
            printf("%s\n", strerror(err));
    }

    munmap(mm, sizeof (*mm));
    if ((unlink("mt_test")) == -1) {
        perror("unlink");
    }
    return 0;
}
