#include <iostream>
#ifndef STRNGBAD_H
#define STRNGBAD_H
class StringBad{
private:
    char *str;
    int len;
    static int num_strings;
public:
    StringBad( const char *s );
    StringBad();
    StringBad( const StringBad & );  //复制构造函数
    StringBad &operator=( const StringBad & );  //赋值运算符
    ~StringBad();
    friend std::ostream &operator<<( std::ostream &os, const StringBad &st );
};
#endif
