#include <string>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
    std::string str("hello world");
    str.compare("hello");

    gettimeofday();
    return 0;
}
