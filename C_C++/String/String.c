#include "String.h"

string string_new(int cate, ...)
{
    if (cate != CHAR_P && cate != MY_STRING)
        return NULL;

    int is_charp = (cate == CHAR_P);
    va_list ap;

    const char *str2;
    size_t size_str2;

    va_start(ap, cate);
    /*
     * Get char * and size
     * */
    if (is_charp) {
        str2 = va_arg(ap, char *);
        if (str2 == NULL)
            return NULL;
        size_str2 = strlen(str2);
    } else {
        string tmp = va_arg(ap, string);
        str2 = string_c_str(tmp);
        size_str2 = string_size(tmp);
    }
    va_end(ap);

    string src = (string) malloc(sizeof(struct string_node));
    src->size = size_str2;
    src->capacity = size_str2 + INIT_LEN;
    src->str = (char *) malloc(src->capacity);

    strcpy(src->str, str2);

    return src;
}


void string_del(string src)
{
    if (NULL == src)
        return;

    free(src->str);
    free(src);
}

void string_append(int cate, string src, ...)
{
    if ((cate != CHAR_P && cate != MY_STRING) || NULL == src)
        return;

    int is_charp = (cate == CHAR_P);
    const char *str2;
    size_t size2;
    va_list ap;
    va_start(ap, src);

    if (is_charp) {
        str2 = va_arg(ap, char *);
        if (NULL == str2)
            return;
        size2 = strlen(str2);
    } else {
        string tmp = va_arg(ap, string);
        if (NULL == str2)
            return;
        str2 = string_c_str(tmp);
        size2 = string_size(tmp);
    }
    va_end(ap);

    src->size += size2;
    if (src->size >= src->capacity) {
        while (src->size >= src->capacity)
            src->capacity *= 2;
        src->str = (char *) realloc(src->str, src->capacity);
    } 
    strcat(src->str, str2);
}

void string_assign(int cate, string src, ...)
{
    if ((cate != CHAR_P && cate != MY_STRING) || src == NULL)
        return;

    int is_charp = (cate == CHAR_P);
    const char *str2;
    size_t size2;
    va_list ap;
    va_start(ap, src);

    if (is_charp) {
        str2 = va_arg(ap, char *);
        if (NULL == str2)
            return;
        size2 = strlen(str2);
    } else {
        string tmp = va_arg(ap, string);
        if (NULL == str2)
            return;
        str2 = string_c_str(tmp);
        size2 = string_size(tmp);
    }
    va_end(ap);

    if (src->capacity <= size2) {
        while (src->capacity <= size2)
            src->capacity *= 2;
        free(src->str);
        src->str = (char *) malloc(src->capacity);
    }
    strcpy(src->str, str2);
    src->size = size2;
}

char string_at(const string src, size_t pos)
{
    if (src == NULL || pos >= string_size(src))
        return '\0';

    return string_c_str(src)[pos];
}

void string_clear(string src)
{
    if (src == NULL)
        return;
    src->str[0] = '\0';
    src->size = 0;
}

int string_compare(const string str1, const string str2)
{
    if (str1 == NULL || NULL == str2)
        return -2;
    return strcmp(string_c_str(str1), string_c_str(str2));
}

int string_empty(const string src)
{
    if (src == NULL)
        return 1;
    return src->size == 0;
}

int string_erase(string src, size_t pos, size_t n)
{
    if (NULL == src || pos >= string_size(src))
        return 0;

    size_t i, j;

    for (i = pos + n, j = pos; i <= src->size; ++i, ++j)
        src->str[j] = src->str[i];

    return 1;
}

size_t string_find(const string src, const char *tar, size_t pos) 
{
    if (NULL == src || tar == NULL || pos >= string_size(src))
        return -1;
    
    const char *p = strstr(src->str + pos, tar);
    return p ? p - src->str : -1;
}

int string_insert(string src, size_t pos, const string dst)
{
    if (src == NULL || dst == NULL || pos > string_size(src))
        return 0;

    const char *str2 = dst->str;
    const size_t size2 = dst->size;
    src->size += size2;

    if (src->size >= src->capacity) {
        while (src->size >= src->capacity)
            src->capacity *= 2;
        src->str = (char *) realloc(src->str, src->capacity);
    }

    int i, j;
    for (i = src->size - size2; i >= pos; --i)
        src->str[i+size2] = src->str[i];
    for (i = 0; i < size2; ++i)
        src->str[i+pos] = str2[i];

    return 1;
}

int string_replace(string src, const char *ori, const char *tar)
{
    /*
     * Algorithm: 
     *  1. Traverse src to count the num of ori's occurrence.
     *  2. Allocate a new memory for src, traverse again src
     *     and copy the character to new memory, if the position is
     *     a occurrence of ori, then copy tar to new memory instead.
     *  3. Over, Complexity is O(n + m*k), k is the cnt.  
     * */
    if (NULL == src || NULL == ori || NULL == tar || '\0' == *ori)
        return 0;       // Failure
    
    size_t pos = 0, cnt_occurance = 0;
    while ((pos = string_find(src, ori, pos)) != -1) {
        ++cnt_occurance;
        ++pos;
    }

    size_t len_src = string_size(src);
    size_t len_ori = strlen(ori);
    size_t len_tar = strlen(tar);
    size_t len_new = len_src + (int) (len_tar - len_ori) * cnt_occurance + 1;
    int i, j;
    char *new_src = (char *) malloc(len_new);

    for (i = 0, j = 0; i < len_src; ) {
        if (strncmp(src->str + i, ori, len_ori) == 0) {
            strcpy(new_src + j, tar);
            j += len_tar;
            i += len_ori;
        } else
            new_src[j++] = (src->str)[i++];
    }
    
    new_src[j] = '\0';
    free(src->str);
    src->str = new_src;
    return 0;
}

string string_substr(const string src, size_t pos)
{
    if (src == NULL || pos > string_size(src))
        return NULL;
    return string_new(CHAR_P, src->str + pos);
}

void string_swap(string *src, string *dst)
{
    if (src == NULL || dst == NULL)
        return;
    string tmp = *src;
    *src = *dst;
    *dst = tmp;
}

const char *string_c_str(string src)
{
    if (NULL == src)
        return NULL;
    return src->str;
}

size_t string_size(const string src)
{
    if (NULL == src)
        return 0;
    return src->size;
}

int string_to_int(int cate, ...)
{
    if (cate != CHAR_P && cate != MY_STRING)
        return 0;

    int is_charp = (cate == CHAR_P);
    va_list ap;

    const char *str;
    size_t len;

    va_start(ap, cate);
    /*
     * Get char * and size
     * */
    if (is_charp) {
        str = va_arg(ap, char *);
        if (str == NULL)
            return 0;
        len = strlen(str);
    } else {
        string tmp = va_arg(ap, string);
        if (tmp == NULL)
            return 0;
        str = string_c_str(tmp);
        len = string_size(tmp);
    }
    va_end(ap);

    int ret = 0;
    int i = 0, sign = 1;

    while (i < len && str[i] == ' ')
        ++i;

    if (str[i] == '+') ++i;
    else if (str[i] == '-') {
        sign = -1;
        ++i;
    }

    for ( ; i < len; ++i) {
        if (str[i] < '0' || str[i] > '9')
            break;
        if (ret > INT_MAX / 10 || ret == INT_MAX / 10 && str[i] - '0' > INT_MAX % 10) {
            if (-1 == sign) return 
                INT_MIN;
            else
                return INT_MAX;
        }
        ret = ret * 10 + str[i] - '0';
    }
    return sign * ret;
}
