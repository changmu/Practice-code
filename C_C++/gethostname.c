#include <myLinux.h>

int main(int argc, char *argv[])
{
    char host[100] = "127.0.0.1";
    int ret = gethostname(host, sizeof(host));
    if (ret < 0) { ERR_EXIT("gethostname"); }
    puts(host);

    struct hostent *hp;
    hp = gethostbyname("127.0.0.1");
    if (hp == NULL) { ERR_EXIT("gethostbyname"); }

    int i = 0;
    while (hp->h_addr_list[i] != NULL) {
       printf("%s\n", inet_ntoa(*(struct in_addr *)hp->h_addr_list[i]));
       ++i;
    }
    return 0;
}
