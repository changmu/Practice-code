/*
   total 348
   drwxrwxr-x  3 qiu qiu   4096 12月 16 16:21 ./
   drwxrwxr-x 12 qiu qiu   4096 12月 16 14:15 ../
   */
#include <myLinux.h>

#define MAX_LEN 1024

static void myLl(const char *pathName);
static void parseStat(struct stat *st, char *buf);

static void getPermissionBits(struct stat *st, char *buf);
static void getLinkNum(struct stat *st, char *buf);
static void getUidAndGid(struct stat *st, char *buf);
static void getFileSize(struct stat *st, char *buf);
static void getMtime(struct stat *st, char *buf);

int main(int argc, char **argv)
{
    if (argc == 1) {
        myLl(".");
    } else {
        while (argc-- > 1)
            myLl(argv[argc]);
    }
    return 0;
}

static void myLl(const char *pathName)
{
    struct stat st;
    char result[MAX_LEN];
    char name[MAX_LEN];

    // ignore check validity
    stat(pathName, &st);
    printf("%s: ", pathName);
    parseStat(&st, result);

    if ((st.st_mode & S_IFMT) == S_IFDIR) {
        DIR *myDir = opendir(pathName);
        struct dirent *d;

        while ((d = readdir(myDir)) != NULL) {
            // ignore check filename length
            sprintf(name, "%s/%s", pathName, d->d_name);
            stat(name, &st);
            printf("%s: ", name);
            parseStat(&st, result);
        }

        closedir(myDir);
    }
}

static void parseStat(struct stat *st, char *buf)
{
    char result[MAX_LEN];

    getPermissionBits(st, result);  strcpy(buf, result);
    getLinkNum(st, result);         strcat(buf, result);
    getUidAndGid(st, result);       strcat(buf, result);
    getFileSize(st, result);        strcat(buf, result);
    getMtime(st, result);           strcat(buf, result);

    puts(buf);
}

static void getPermissionBits(struct stat *st, char *buf)
{
    switch (st->st_mode & S_IFMT) {
        case S_IFBLK:   buf[0] = 'b';   break;
        case S_IFCHR:   buf[0] = 'c';   break;
        case S_IFDIR:   buf[0] = 'd';   break;
        case S_IFIFO:   buf[0] = 'f';   break;
        case S_IFLNK:   buf[0] = 's';   break;
        case S_IFREG:   buf[0] = '-';   break;
        case S_IFSOCK:  buf[0] = 'S';   break;
    }

    int i = 1;

    buf[i++] = (st->st_mode & S_IRUSR) ? 'r' : '-';
    buf[i++] = (st->st_mode & S_IWUSR) ? 'w' : '-';
    buf[i++] = (st->st_mode & S_IXUSR) ? 'x' : '-';
    buf[i++] = (st->st_mode & S_IRGRP) ? 'r' : '-';
    buf[i++] = (st->st_mode & S_IWGRP) ? 'w' : '-';
    buf[i++] = (st->st_mode & S_IXGRP) ? 'x' : '-';
    buf[i++] = (st->st_mode & S_IROTH) ? 'r' : '-';
    buf[i++] = (st->st_mode & S_IWOTH) ? 'w' : '-';
    buf[i++] = (st->st_mode & S_IXOTH) ? 'x' : '-';

    buf[i++] = '\t';
    buf[i] = '\0';
}

static void getLinkNum(struct stat *st, char *buf)
{
    sprintf(buf, "%d\t", (int) st->st_nlink);
}

static void getUidAndGid(struct stat *st, char *buf)
{
    strcpy(buf, getpwuid(st->st_uid)->pw_name);
    strcpy(buf, getgrgid(st->st_gid)->gr_name);
}

static void getFileSize(struct stat *st, char *buf)
{
    char str[1024];
    mySizeFormat(st->st_size, str);
    sprintf(buf, "\t%s\t", str);
}

static void getMtime(struct stat *st, char *buf)
{
    strcpy(buf, "2015年12月\t");   
}
