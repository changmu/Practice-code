#include <myLinux.h>

#define MAX_LEN 1024

void myRm(const char *pathName);
void rmPath(const char *pathName, const struct stat *st);

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <pathName>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int i;

    for (i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], ".") || !strcmp(argv[i], "..")) {
            fprintf(stderr, "illegal arguments %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
        myRm(argv[i]);
    }

    return 0;
}

void myRm(const char *pathName)
{
    struct stat st;

    if (lstat(pathName, &st) == -1) {
        perror("stat: ");
        exit(EXIT_FAILURE);
    }

    rmPath(pathName, &st);
}

void rmPath(const char *pathName, const struct stat *st)
{
    int flag = st->st_mode & S_IFMT;
    if (flag == S_IFREG || flag == S_IFLNK) {
        unlink(pathName);
        return;
    }

    // handle dir
    if (flag != S_IFDIR) {
        fprintf(stderr, "couldn't remove this kind of file: %s\n", pathName);
        exit(EXIT_FAILURE);
    }

    // ignore check
    DIR *myDir = opendir(pathName);
    struct dirent *d;
    char name[MAX_LEN];
    struct stat st_sub;

    while ((d = readdir(myDir)) != NULL) {
        if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")) {
            continue;
        }
        sprintf(name, "%s/%s", pathName, d->d_name);
        // ignore check
        lstat(name, &st_sub);

        rmPath(name, &st_sub);
    }

    // remove empty dir
    rmdir(pathName);
}
