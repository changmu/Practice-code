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
    /* ��ʼ����������Ҫ�ȴ� */
    istream_iterator<int> in_iter(cin), eof;
    ostream_iterator<int> out_iter(cout, " ");
    cout << "Input an integer: ";
    // cin >> N;
    N = *in_iter++; // ��ȡN��in_iterָ����һ��λ�ã��ȴ��ٴ����룬���Գ����ڴ�ͣ��
    cout << "N is " << N << endl;
    raw.clear();
    while (N--) *back_inserter(raw) = *in_iter++; // ͬ����ȡ�����һ����������ͣ�͵ȴ�����
    copy(raw.begin(), raw.end(), out_iter);
    // N = *in_iter;
    cout << "N is " << N << endl;
    return 0;
}
