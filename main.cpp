#include <iostream>
#include <set>
#include <random>
#include "AVLtest.h"
#include "sortTest.h"

int main()
{
    std::cout << "Hello, World!" << std::endl;

    std::default_random_engine e{};
    std::uniform_int_distribution<int> dis(0, 100);
    std::set<int> s;
    for (int i = 0; i < 20; ++i)
    {
        s.insert(dis(e));
    }
    int *arr = static_cast<int *>(malloc(s.size() * sizeof(int)));
    size_t i = 0;
    for (auto n : s)
    {
        arr[i++] = n;
    }
    AVLTest(arr, s.size());
    return 0;
}
