#include <cstring>
#include "strngbad.h"
using namespace std;

int StringBad::num_strings = 0;

StringBad::StringBad( const char *s ){
    len = strlen( s );
    str = new char[len + 1];
    strcpy( str, s );
    ++num_strings;
    cout << num_strings << ": \"" << str << "\" object created\n";
}

StringBad::StringBad(){
    len = 4;
    str = new char[4];
    strcpy( str, "C++" );
    ++num_strings;
    cout << num_strings << ": \"" << str << "\" default object created\n";
}

StringBad::~StringBad(){
    cout << "\"" << str << "\" object delete, ";
    --num_strings;
    cout << num_strings << " left\n";
    delete [] str;
}

ostream &operator<<( ostream &os, const StringBad &st ){
    os << st.str;
    return os;
}

StringBad::StringBad( const StringBad &s ){
    ++num_strings;
    len = strlen( s.str );
    str = new char[len + 1];
    strcpy( str, s.str );
    cout << num_strings << ": \"" << str << "\" object created\n";
}

StringBad &StringBad::operator=( const StringBad &st ){ //因为赋值操作不创建新的对象，所以不增加num_strings的值
    if( this == &st )
        return *this;
    delete [] str;
    len = strlen( st.str );
    str = new char[len +1];
    strcpy( str, st.str );
    return *this;
}
