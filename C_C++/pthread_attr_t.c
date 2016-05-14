#include <myLinux.h>

#define SIZE 0x10000

int print_ntimes(char *str)
{
    sleep(1);
    printf("%s\n", str);
    return 0;
}

void *th_fun(void *arg)
{
    int n = 3;
    while (n--) {
        printf("hello qiu\n");
    }
    return (void *) 1;
}

int main(int argc, char *argv[])
{
    int             err;
    int             detachstate;
    int             i = 1;
    size_t          stacksize;
    void            *stackaddr;
    pthread_t       tid;
    pthread_attr_t  attr;

    pthread_attr_init(&attr);

    pthread_attr_getstack(&attr, &stackaddr, &stacksize);
    pthread_attr_getdetachstate(&attr, &detachstate);

    printf("stackaddr = %p\n", stackaddr);
    printf("stacksize = %x\n", (int) stacksize);
    if (detachstate == PTHREAD_CREATE_DETACHED)
        printf("thread detached\n");
    else if (detachstate == PTHREAD_CREATE_JOINABLE)
        printf("thread joinable\n");
    else
        printf("unknow\n");

    /* set detach property */
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    while (1) {
        /* malloc memory on heap, set stack init size and addr */
        stackaddr = malloc(SIZE);
        if (stackaddr == NULL) {
            perror("malloc");
            exit(1);
        }
        stacksize = SIZE;
        pthread_attr_setstack(&attr, stackaddr, stacksize);

        err = pthread_create(&tid, &attr, th_fun, NULL);
        if (err) {
            printf("%s\n", strerror(err));
            exit(1);
        }
        printf("%d\n", i++);
    }

    pthread_attr_destroy(&attr);

    return 0;
}
