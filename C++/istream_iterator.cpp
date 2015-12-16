#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

vector<int> raw;
int N;

int main()
{
    /* 初始化缓冲区需要等待 */
    istream_iterator<int> in_iter(cin), eof;
    ostream_iterator<int> out_iter(cout, " ");
    cout << "Input an integer: ";
    // cin >> N;
    N = *in_iter++; // 读取N后in_iter指向下一个位置，等待再次输入，所以程序在此停滞
    cout << "N is " << N << endl;
    raw.clear();
    while (N--) *back_inserter(raw) = *in_iter++; // 同理，读取完最后一个数后依旧停滞等待输入
    copy(raw.begin(), raw.end(), out_iter);
    // N = *in_iter;
    cout << "N is " << N << endl;
    return 0;
}
