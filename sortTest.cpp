//
// Created by 0x0 on 4/30/21.
//

#include "sortTest.h"

#include <iostream>
#include <vector>
#include <chrono>

#include "heapsort.h"
#include "quicksort.h"
#include "insertion_sort.h"

std::vector<std::vector<int>> GenerateTestCases(size_t size)
{
    static std::default_random_engine re(std::random_device{}());

    static std::uniform_int_distribution<int>
        INT(std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max());

    std::uniform_int_distribution<size_t> SIZE(0, size - 1);

    std::vector<std::vector<int>> result;
    for (size_t i = 0; i < size; ++i)
    {
        result.emplace_back();
        for (size_t j = 0, maxJ = SIZE(re); j < maxJ; ++j)
        {
            result.back().emplace_back(INT(re));
        }
    }
    return result;
}

template<typename T>
auto testSort(std::function<void(T &)> f,
              std::vector<T> testCases)
{
    size_t count = 0;
    auto start = std::chrono::steady_clock::now();
    for (auto &v : testCases)
    {
        f(v);
        std::cout << ++count << ' ' << std::flush;
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "\n"
              << (end - start).count() << std::endl;
    return testCases;
}

int sortTest()
{
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios::sync_with_stdio(false);

    auto cases = GenerateTestCases(3000);

    auto cmp = [](int lhs, int rhs) -> bool {
        return lhs > rhs;
    };

    auto result = testSort(
        std::function<void(std::vector<int> &)>(
            [&](std::vector<int> &v) {
                std::sort(v.begin(), v.end(), cmp);
            }),
        cases);
    std::cout << std::endl;

    std::cout << (result == testSort(
        std::function<void(std::vector<int> &)>(
            [&](std::vector<int> &v) {
                quick_sort_tail(v.begin(), v.end(), cmp);
            }),
        cases)) << std::endl;

    std::cout << (result == testSort(
        std::function<void(std::vector<int> &)>(
            [&](std::vector<int> &v) {
                quicksort(v.begin(), v.end(), cmp);
            }),
        cases)) << std::endl;

    std::cout << (result == testSort(
        std::function<void(std::vector<int> &)>(
            [&](std::vector<int> &v) {
                heapsort(v.begin(), v.end(), cmp);
            }),
        cases)) << std::endl;

    std::cout << (result == testSort(
        std::function<void(std::vector<int> &)>(
            [&](std::vector<int> &v) {
                insertion_sort(v.begin(), v.end(), cmp);
            }),
        cases)) << std::endl;

//    quick_sort_tail(cases[0].begin(), cases[0].end(), cmp);
//    for (auto v : cases[0])
//    {
//        std::cout << v << ' ';
//    }

    std::cout << std::endl;
    return 0;
}
