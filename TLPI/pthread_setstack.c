#include <myLinux.h>

#define SIZE 0x100000

int print_ntimes(char *str)
{
    printf("%s", str);
    sleep(1);
    return 0;
}

void *thr_fn(void *arg)
{
    int n = 3;
    while (n--)
        print_ntimes("hello");
    return (void *) 1;
}

int main(int argc, char *argv[])
{
    pthread_t       tid;
    int             err;
    int             cnt = 1;
    int             detachstate;
    pthread_attr_t  attr;
    size_t          stacksize;
    void            *stackaddr;

    pthread_attr_init(&attr);

    pthread_attr_getstack(&attr, &stackaddr, &stacksize);
    printf("stackaddr=%p\n", stackaddr);
    printf("stacksize=%x\n", (unsigned) stacksize);

    pthread_attr_getdetachstate(&attr, &detachstate);
    if (detachstate == PTHREAD_CREATE_DETACHED)
        printf("thread detached\n");
    else if (detachstate == PTHREAD_CREATE_JOINABLE)
        printf("thread join\n");
    else
        printf("thread unknown\n");

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    while (1) {
        stackaddr = malloc(SIZE);
        if (stackaddr == NULL) {
            perror("malloc");
            exit(1);
        }

        stacksize = SIZE;
        pthread_attr_setstack(&attr, stackaddr, stacksize);

        err = pthread_create(&tid, &attr, thr_fn, NULL);
        if (err) {
            printf("%s\n", strerror(err));
            exit(1);
        }
        // pthread_join(tid, NULL);
        
        printf("%d\n", cnt++);
    }

    pthread_attr_destroy(&attr);

    return 0;
}
