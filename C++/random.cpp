#include <random>
#include <iostream>

int main()
{
    std::default_random_engine e;
    for (size_t i = 0; i < 012; ++i) {
        std::cout << "i: " << i << " --- " << e() << std::endl;
    }
    return 0;
}
