#include "str.h"

void str_trim_crlf(char *str)
{
        char *p = &str[strlen(str) - 1];
        while (isspace(*p))
                *p-- = '\0';
}

void str_split(const char *str, char *left, char *right, char c)
{
        char *p = strchr(str, c);
        if (p == NULL)
                strcpy(left, str);
        else {
                strncpy(left, str, p - str);
                strcpy(right, p + 1);
        }
}

int str_all_space(const char *str)
{
        while (*str) {
                if (!isspace(*str++))
                        return 0;
        }

        return 1;
}

void str_upper(char *str)
{
        while (*str) {
                *str = toupper(*str);
                ++str;
        }
}

long long str_to_longlong(const char *str)
{
        return atoll(str);
}

unsigned int str_octal_to_uint(const char *str)
{
        unsigned int result = 0;

        while (*str) {
                int digit = *str++;
                if (!isdigit(digit) || digit > '7')
                        break;

                result = result * 8 + digit - '0';
        }

        return result;
}
