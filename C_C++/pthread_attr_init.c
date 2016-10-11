#include <myLinux.h>

void *thr_fn(void *arg)
{
    int n = 10;

    while (n--) {
        printf("%x   %d\n", (unsigned) pthread_self(), n);
        sleep(1);
    }

    return (void *) 1;
}

int main(int argc, char *argv[])
{
    pthread_t       tid;
    pthread_attr_t  attr;
    int             err;

    pthread_attr_init(&attr);
    /* int detachstate: PTHREAD_CREATE_DETACHED PTHREAD_CREATE_JOINABLE */
    /* 设置分离属性 */
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&tid, &attr, thr_fn, NULL);

    err = pthread_join(tid, NULL);
    while (1)
        if (err) {
            printf("... %s\n", strerror(err));
            pthread_exit((void *) 1);
        }

    return 0;
}
