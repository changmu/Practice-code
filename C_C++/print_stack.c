#include "myLinux.h"


static void function3() { 
        print_stack();
        exit(1);
}

void function2() { function3(); }
void function1() { function2(); }
void function() { function1(); }

int main(int argc, char *argv[])
{
        function();
        return 0;
}
