#include "myLinux.h"

void *thread_routine(void *arg)
{
        int i;
        for (i = 0; i < 20; ++i) {
                printf("B");
                fflush(stdout);
                usleep(20);
                
                if (i == 30)
                        pthread_exit("ABC");
        }
        return "def";
}

int main(int argc, char *argv[])
{
        pthread_t tid;
        int ret;
        ret = pthread_create(&tid, NULL, thread_routine, NULL);
        if (ret != 0) {
                fprintf(stderr, "pthread_create:%s\n", strerror(ret));
                exit(EXIT_FAILURE);
        }

        int i;
        for (i = 0; i < 20; ++i) {
                printf("A");
                fflush(stdout);
                usleep(20);
        }

        // sleep(1);

        void *value;
        ret = pthread_join(tid, &value);
        if (ret != 0) {
                fprintf(stderr, "pthread_join:%s\n", strerror(ret));
                exit(EXIT_FAILURE);
        }

        printf("\n");

        printf("return msg=%s\n", (char *) value);

        return 0;
}
