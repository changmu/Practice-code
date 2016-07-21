#include <myLinux.h>

#define MAXN 5000000

typedef long long LL;

LL sum, N;

void *thr_fn(void *arg)
{
    LL i;
    for (i = 0; i < N; ++i)
        sum++;
    return (void *) 1;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s num\n", argv[0]);
        return 1;
    }
    int num = atoi(argv[1]);
    int j, err;
    pthread_t tid;

    N = MAXN / num;

    for (j = 0; j < num; ++j) {
        err = pthread_create(&tid, NULL, thr_fn, NULL);
        if (err) {
            printf("%s..\n", strerror(err));
        }
    }

    // for (j = 0; j < num; ++j)
        // pthread_join(tid[j], NULL);
    printf("%lld\n", sum);

    return 0;
}
