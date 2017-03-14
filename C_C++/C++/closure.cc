#include <stdio.h>

#include <functional>
#include <memory>

using namespace std;

class Int 
{
public:
    Int(int v)
      : v_(v)
    {
        printf("ctor()\n");
    }
    ~Int()
    {
        printf("dtor()\n"); 
    }
    int increase()
    {
        return ++v_;
    }

private:
    int v_;
};

int main()
{
    {
        auto generateClosure = [] {
            shared_ptr<Int> count(new Int(0));
            auto get = [count] {
                return count->increase();
            };
            return get;
        };

        auto func = generateClosure();
        printf("count = %d\n", func());
        printf("count = %d\n", func());

        auto func2 = generateClosure();
        printf("count = %d\n", func2());
        printf("count = %d\n", func2());
    }

    printf("end...\n");
    return 0;
}
