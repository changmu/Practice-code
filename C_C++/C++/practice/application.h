#pragma once

#include <iostream>

template<typename T>
class A {
public:
    T a, b, c;
    A() = default;
    void f() { std::cout << a << std::endl; }
};

template<typename T>
T compare(const T&, const T&)
{
    return T();
}
