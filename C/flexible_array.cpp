#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct st_node {
    int a, b;
    double c;
    int arr[];
} st_node;

int main()
{
    st_node *p = (st_node *)malloc(sizeof(st_node) + sizeof(int) * 100);
    p->arr[99] = 123;
    printf("%d\n", p->arr[99]);
    printf("%d\n", p->arr[0]);
    free(p);
    return 0;
}
