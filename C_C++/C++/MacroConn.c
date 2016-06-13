#include <stdio.h>
#include <string.h>

#pragma pack(8)

// #把宏参数变为字符串
// ##连接两个参数
#define STR(x) #x
#define CONS(a, b) (int)(a##e##b)
#define ARRLEN(a) (sizeof(a) / sizeof(*a))

union u2 {
    char a[13];
    int b;
};

int main()
{
    printf(STR(abcdef) "\n"); 
    printf("%d\n", CONS(2, 3));
    printf("%zu\n", sizeof(union u2));

    int arr[5][6];
    printf("%zu:%zu\n", ARRLEN(arr), ARRLEN(arr[2]));
    return 0;
}
