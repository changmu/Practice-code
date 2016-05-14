#include <iostream>
#include <cstring>
#include "workermi.h"
#define SIZE 5
using std::cin;
using std::cout;
using std::endl;
using std::strchr;

int main(){
    Worker *lolas[SIZE];
    int ct;
    for( ct = 0; ct != SIZE; ++ct ){
        char choice;
        cout << "Enter the employee category:\n"
             << "w: waiter s: singer "
             << "t: singing waiter q: quit\n";
        cin >> choice;
        while( strchr( "wstq", choice ) == NULL ){
            cout << "Please enter a w, s, t, or q: ";
            cin >> choice;
        }
        if( 'q' == choice )
            break;
        switch( choice ){
            case 'w': lolas[ct] = new Waiter; break;
            case 's': lolas[ct] = new Singer; break;
            case 't': lolas[ct] = new SingingWaiter; break;
        }
        cin.get();
        lolas[ct]->Set();
    }
    cout << "\nHere is your staff:\n";
    int i;
    for( i = 0; i != ct; ++i ){
        cout << endl;
        lolas[i]->show();
    }
    for( i = 0; i != ct; ++i )
        delete lolas[i];
    cout << "Bye.\n";
    return 0;
}
