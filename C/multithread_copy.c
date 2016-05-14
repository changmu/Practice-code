#include <myLinux.h>

struct Copy_node {
    char    *src_addr;
    char    *dst_addr;
    size_t  thread_no;
    size_t  block_size;
    size_t  *progress_bar;
};

static void *thr_fn(void *arg)
{
    struct Copy_node *node = arg;

    while (node->block_size--) {
        node->dst_addr[node->block_size] = node->src_addr[node->block_size];
        ++node->progress_bar[node->thread_no];
    }

    return (void *) 1;
}

static void draw_progress_bar(size_t copyed_size, size_t total_size)
{
    size_t progress = (size_t) ((double) copyed_size / total_size * 100);
    int i;

    for (i = 0; i != progress; ++i)
        putchar('=');
    printf("[%d%%]\r", i);
}

int main(int argc, char *argv[])
{
    size_t thread_num = 4;  // default
    /* process args */
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "usage: %s <src> <dst> [thread_num]\n", argv[0]);
        exit(1);
    }
    if (argc == 4)
        thread_num = atoi(argv[3]);

    ssize_t block_size;
    int src_fd;
    ssize_t src_size;
    int dst_fd;
    ssize_t dst_size;
    size_t *progress_bar;
    char *src_addr;
    char *dst_addr;
    int i;
    size_t copyed_size;

    pthread_t tid;
    pthread_attr_t attr;

    /* open srcfile and get its size */
    if ((src_fd = open(argv[1], O_RDONLY)) == -1)
        errExit("open src");
    if ((src_size = lseek(src_fd, 0, SEEK_END)) == -1)
        errExit("lseek src");
    dst_size = src_size;
    //! ignore checking src_size and adjusting blocksize

    /* open dstfile and lseek it */
    if ((dst_fd = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, 0666)) == -1)
        errExit("open dst");
    if (lseek(dst_fd, dst_size - 1, SEEK_SET) == -1)
        errExit("lseek dst");
    if (write(dst_fd, "\0", 1) == -1)
        errExit("write dst");

    /* mmap src and dst */
    src_addr = mmap(NULL, src_size, PROT_READ, MAP_PRIVATE, src_fd, 0);
    if (src_addr == (void *) -1)
        errExit("mmap src");
    dst_addr = mmap(NULL, dst_size, PROT_WRITE, MAP_SHARED, dst_fd, 0);
    if (dst_addr == (void *) -1)
        errExit("mmap dst");

    /* calculate blocksize */
    if (thread_num == 0)                // check validity
        ++thread_num;
    block_size = src_size / thread_num;  // the last thread should plus the remainder
    progress_bar = calloc(thread_num, sizeof (*progress_bar));
    if (progress_bar == NULL)
        errExit("calloc progress_bar");

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    struct Copy_node **node_arr = malloc(sizeof (struct Copy_node) * thread_num);

    for (i = 0; i != thread_num; ++i) {
        node_arr[i] = malloc(sizeof (struct Copy_node));
        struct Copy_node *node = node_arr[i];

        node->src_addr      = src_addr + block_size * i;
        node->dst_addr      = dst_addr + block_size * i;
        node->thread_no     = i;
        node->block_size    = block_size;
        if (i == thread_num - 1)
            node->block_size += src_size % thread_num;
        node->progress_bar  = progress_bar;
    
        pthread_create(&tid, &attr, thr_fn, node);
    }

    /* make statistics */
    do {
        copyed_size = 0;
        for (i = 0; i != thread_num; ++i)
            copyed_size += progress_bar[i];

        draw_progress_bar(copyed_size, src_size);
        usleep(1000);
    } while (copyed_size != src_size);
    printf("\n");

    /* resource recovery */
    munmap(src_addr, src_size);
    munmap(dst_addr, dst_size);
    free(progress_bar);
    pthread_attr_destroy(&attr);
    for (i = 0; i != thread_num; ++i)
        free(node_arr[i]);
    free(node_arr);

#ifdef DEBUG
    printf("TEST: ..size: %lu\n", src_size);
#endif
    return 0;
}
