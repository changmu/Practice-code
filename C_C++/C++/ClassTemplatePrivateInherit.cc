#include <cstdio>
#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <vector>

using namespace std;

template<typename T>
class SquareMatrixBase {
protected:
    void invert(size_t matrixSize);
};

template<typename T, size_t n>
class SquareMatrix: private SquareMatrixBase<T> {
private:
    using SquareMatrixBase<T>::invert;
public:
    void invert() { this->invert(); }
};

int main()
{
     
    return 0;
}
