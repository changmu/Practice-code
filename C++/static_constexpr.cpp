#include <iostream>
#include <string>

using namespace std;

class Student {
private:
    string name;
    int age;
    static const int num;
    static int test;
    static constexpr int test2 = 2 + 3;
public:
    Student() : name("changmu"), age(22) {}
    explicit Student(int a) : Student() {}
    void print() {
        cout << num << endl;
        cout << test2 << endl;
    }
};
int Student::test = 3;
const int Student::num = 2;

int main()
{
    Student stu;
    stu.print();
    return 0;
}
