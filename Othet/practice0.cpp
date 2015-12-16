#include <stdio.h>
#include <string.h>

const int maxn = 1e5 + 10;
char str1[maxn<<1], str2[maxn];

int main()
{
    while (scanf("%s", str1) == 1) {
        strcpy(str2, str1);
        strcat(str1, str2);
        scanf("%s", str2);
        puts(strstr(str1, str2) ? "yes" : "no");
    }
    return 0;
}
