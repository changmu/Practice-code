#include "String.h"

int main()
{
    string str = string_new(CHAR_P, "Hello, Heurei.");
    puts(string_c_str(str));
    string str2 = string_new(MY_STRING, str);
    string_del(str2);

    string_append(CHAR_P, str, " Nice weather.");
    puts(string_c_str(str));
    str2 = string_new(CHAR_P, " Isn't it?");
    string_append(MY_STRING, str, str2);
    puts(string_c_str(str));

    string str3 = string_new(MY_STRING, str);
    string_assign(CHAR_P, str3, "Yes, it is.");
    puts(string_c_str(str3));

    putchar(string_at(str3, 0));
    string_clear(str3);

    string str4 = string_new(CHAR_P, "");
    string_compare(str3, str4);
    string_empty(str4) && puts(" Empty");

    string str5 = string_new(MY_STRING, str);
    string_erase(str5, 0, 5);
    puts(string_c_str(str5));

    string str6 = string_new(MY_STRING, str);
    printf("..%lu\n", string_find(str6, "ll", 0));

    string_insert(str6, 1, str);
    puts(string_c_str(str6));
    string_replace(str6, "ll", "LLL");
    puts(string_c_str(str6));

    puts(string_c_str(string_substr(str, 5)));

    string_swap(&str, &str6);
    puts(string_c_str(str6));

    printf("%d\t%d\n", string_to_int(CHAR_P, "-123"), string_to_int(MY_STRING, string_new(CHAR_P, "+345")));

    return 0;
}
