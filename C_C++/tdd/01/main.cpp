#include <stdio.h>
#include <gtest.h>

class TestEnvironment {
public:
    int Run()
    {
        testMultiplication();
        return 0;
    }

    void testMultiplication() {
        Dollar five(5);
        five.times(2);
        assertEquals(10, five.amount);
    }

private:
    class Dollar {
    public:
        Dollar(int v) {

        }
        int amount;
        void times(int n) {

        }
    };

    void assertEquals(int a, int b) {

    }
};

int main()
{
    TestEnvironment t;
    t.Run();
    return 0;
}

