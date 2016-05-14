/*
 * 递归遍历目录，输出文件名和大小
 * */
#include <myLinux.h>

#define MAX_PATH 1024

void readNameAndSize(const char *pathName);
void walkDir(const char *dirName, void (*fun)(const char *pathName));

int main(int argc, char **argv)
{
    if (argc == 1) {
        readNameAndSize(".");
    } else {
        while (argc-- > 1)
            readNameAndSize(argv[argc]);
    }
    return 0;
}

void readNameAndSize(const char *pathName)
{
    struct stat stbuf;

    if (stat(pathName, &stbuf) == -1) {
        perror(pathName);
        printf("%d..\n", __LINE__);
        exit(1);
    }

    char buf[MAX_PATH];
    mySizeFormat(stbuf.st_size, buf);
    printf("%s: ", pathName);
    printf("%s\n", buf);
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
        walkDir(pathName, readNameAndSize);
}

void walkDir(const char *dirName, void (*fun)(const char *pathName))
{
    DIR *dir = opendir(dirName);
    if (dir == NULL) { perror("opendir"); exit(EXIT_FAILURE); }

    struct dirent *d;
    char name[MAX_PATH];

    while ((d = readdir(dir)) != NULL) {
        if (!strcmp(".", d->d_name) || !strcmp("..", d->d_name)) continue;
        if (strlen(dirName) + strlen(d->d_name) + 2 > MAX_PATH) {
            fprintf(stderr, "path name too long: %s/%s\n", dirName, d->d_name);
        } else {
            strcpy(name, dirName);
            strcat(name, "/");
            strcat(name, d->d_name);
            (*fun)(name);
        }
    }
    closedir(dir);
}
