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
    StringBad( const StringBad & );  //���ƹ��캯��
    StringBad &operator=( const StringBad & );  //��ֵ�����
    ~StringBad();
    friend std::ostream &operator<<( std::ostream &os, const StringBad &st );
};
#endif
