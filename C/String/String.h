#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

#define INIT_LEN 2
#define CHAR_P 0
#define MY_STRING 1

typedef struct string_node {
    char *str;
    size_t size;
    size_t capacity;
} string_node;
typedef string_node *string;

/*
 * cate = 0 means the second arg is a char *,
 * cate = 1 means that is a string
 * */
extern string      string_new(int cate, ...);
extern void        string_del(string src);
extern void        string_append(int cate, string src, ...);
extern void        string_assign(int cate, string src, ...);
extern char        string_at(const string src, size_t pos);
extern void        string_clear(string src);
extern int         string_compare(const string, const string);
extern int         string_empty(const string src);
extern int         string_erase(string src, size_t pos, size_t n);
extern size_t      string_find(const string src, const char *tar, size_t pos);
extern int         string_insert(string src, size_t pos, const string dst);
extern size_t      string_size(const string src);
extern int         string_replace(string src, const char *ori, const char *tar);
extern string      string_substr(const string src, size_t pos);
extern void        string_swap(string *src, string *dst);
extern const char *string_c_str(string src);
extern int         string_to_int(int cate, ...);

#endif
