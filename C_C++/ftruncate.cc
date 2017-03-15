#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

const char sam[] = "hello world.";
const char sam2[] = "hello nodejs.";

int main()
{
    int fd = open("tmp.txt", O_RDWR | O_CREAT, 0666);
    lseek(fd, 0, SEEK_END);
    write(fd, sam, sizeof sam);
    ftruncate(fd, 0);
    lseek(fd, 0, SEEK_SET);

    write(fd, sam2, sizeof sam2);

    close(fd);
    return 0;
}
