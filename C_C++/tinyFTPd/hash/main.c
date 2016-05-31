#include "myLinux.h"
#include "hash.h"

typedef struct stu {
        char sno[5];
        char name[32];
        int age;
} stu_t;

typedef struct stu2 {
        int sno;
        char name[32];
        int age;
} stu2_t;

typedef struct hash_node {
        enum entry_status status;
        void *key;
        void *value;
} hash_node_t;

unsigned hash_str(unsigned buckets, void *key)
{
        char *sno = (char *) key;
        unsigned index = 0;
        while (*sno) {
                index = *sno++ + 4 * index;
        }
        return index % buckets;
}

unsigned hash_str2(unsigned buckets, void *key)
{
        int *sno = (int *) key;
        
        return *sno % buckets;
}

int main(int argc, char *argv[])
{
        /*stu_t stu_arr[] = {
                {"1234", "AAAA", 20},
                {"4568", "AAAA", 22},
                {"6789", "AAAA", 19},
        };

        hash_t *hash = hash_alloc(256, hash_str);
        int size = ARRLEN(stu_arr);
        int i;
        for (i = 0; i < size; ++i) {
                hash_add_entry(hash, stu_arr[i].sno, strlen(stu_arr[i].sno), &stu_arr[i], sizeof(stu_arr[i]));
        }

        stu_t *s = hash_lookup_entry(hash, "4568", 4);
        if (s) {
                printf("%s %s %d\n", s->sno, s->name, s->age);
        } else {
                printf("not found\n");
        }

        hash_free_entry(hash, "1234", 4);
        s = hash_lookup_entry(hash, "1234", 4);
        if (s) {
                printf("%s %s %d\n", s->sno, s->name, s->age);
        } else {
                printf("not found\n");
        }*/
        stu2_t stu_arr[] = {
                {1234, "AAAA", 20},
                {4568, "AAAA", 22},
                {6789, "AAAA", 19},
        };

        hash_t *hash = hash_alloc(256, hash_str2);
        int size = ARRLEN(stu_arr);
        int i;
        for (i = 0; i < size; ++i) {
                hash_add_entry(hash, &stu_arr[i].sno, sizeof(stu_arr[i].sno), &stu_arr[i], sizeof(stu_arr[i]));
        }

        int sno = 4568;
        stu2_t *s = hash_lookup_entry(hash, &sno, 4);
        if (s) {
                printf("%d %s %d\n", s->sno, s->name, s->age);
        } else {
                printf("not found\n");
        }

        sno = 1234;
        hash_free_entry(hash, &sno, 4);
        s = hash_lookup_entry(hash, &sno, 4);
        if (s) {
                printf("%d %s %d\n", s->sno, s->name, s->age);
        } else {
                printf("not found\n");
        }

        return 0;
}