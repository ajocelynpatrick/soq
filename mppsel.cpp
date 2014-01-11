#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>  // setw
#include <utility>  // swap
#include <cstdlib>  // rand, srand

using namespace std;

template<class T>
static void check_partition(T *a, size_t start, size_t end, size_t rank)
{
    int ok = 1;
    cout << "s = " << start << "; e = " << end << "; r = " << rank << endl;
    assert(start < end);
    assert(rank >= start && rank <= end);
    for (size_t i = start; i < rank; i++)
    {
        if (a[i] > a[rank])
        {
            ok = 0;
            cout << "BUG: a[" << i << "] = " << a[i]
                 << " should be no larger than a["
                 << rank << "] = " << a[rank] << "\n";
        }
    }
    for (size_t i = rank + 1; i < end; i++)
    {
        if (a[i] < a[rank])
        {
            ok = 0;
            cout << "BUG: a[" << i << "] = " << a[i]
                 << " should be no smaller than a["
                 << rank << "] = " << a[rank] << "\n";
        }
    }
    if (!ok)
        assert(0);
}

template<class T>
void printArray(char const *tag, T *A, size_t l, size_t u)
{
    size_t i;
    size_t N = u - l + 1;
    int const maxw = 15;
    cout << tag << ": (" << l << "," << u << ")";
    if (N > maxw)
        cout << '\n';
    for (i = 0; i < N; i++)
    {
        cout << ' ' << setw(3) << A[i+l];
        if (i % maxw == maxw - 1)
            cout << '\n';
    }
    if (i % maxw != 0)
        cout << '\n';
}

template<class T>
size_t partition(T *a, size_t l, size_t u)        // u = max valid index
{
    printArray("--->> partition()", a, l, u);
    size_t m = l;
    swap(a[(u+l) / 2], a[l]);
    T p = a[l];
    cout << "Pivot: [" << (u+1) / 2 << "] = " << p << endl;
    printArray("--0-- partition()", a, l, u);

    for (size_t i = l+1; i <= u; i++)
    {
        if (a[i] < p)
        {
            ++m;
            cout << "swap (" << i << "," << m << ")<=>(" << a[i] << "," << a[m] << ")\n";
            swap(a[i], a[m]);
            printArray("--1-- partition()", a, l, u);
        }
    }

    printArray("--2-- partition()", a, l, u);
    swap(a[l], a[m]);
    printArray("<<--- partition()", a, l, u);
    check_partition(a, l, u, m);
    cout << "Returning: " << m << endl;
    return m;
}

template<class T>
void Select(T *a, size_t l, size_t u, size_t k)
{
    assert(l <= k && k <= u);
    if (l < u)
    {
        size_t m = partition(a, l, u);
        if (k < m)
            Select(a, l, m-1, k);
        else if (k > m)
            Select(a, m+1, u, k);
    }
}

int main()
{
    int A[] =
    {
        10000000,
        96, 4, 10, -35, 55,   6, 6, 6, -67,  0,   2,  34,  6, 4,
        56, 2,  7,  -9, 45, -27, 5, 7,   8, 94, -99, -98, 99,
    };
    const size_t A_SIZE = sizeof(A) / sizeof(A[0]);
    for (size_t i = 1; i < A_SIZE; i++)
    {
        int B[A_SIZE];
        memmove(B, A, sizeof(B));
        cout << "Rank [" << i << "]" << endl;
        Select(B, 1, A_SIZE-1, i);
        cout << "Rank [" << i << "] = " << B[i] << endl;
        printArray("Finish", B, 1, A_SIZE-1);
        check_partition(B, 1, A_SIZE-1, i);
        cout << '\n';
    }
    return 0;
}

