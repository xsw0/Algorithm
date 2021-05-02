//
// Created by 0x0 on 4/30/21.
//

#include "AVLtest.h"

#include <iostream>
#include <set>
#include <random>
#include <string>
#include <vector>
#include <queue>
#include <iomanip>

#include "AVL.h"

std::string avl_log;

void PreOrderTraverse(const struct AVLImpl *avl, void(*function)(const struct AVLImpl *))
{
    if (avl)
    {
        function(avl);
        PreOrderTraverse(AVL_left(avl), function);
        PreOrderTraverse(AVL_right(avl), function);
    }
}

void InOrderTraverse(const struct AVLImpl *avl, void(*function)(const struct AVLImpl *))
{
    if (avl)
    {
        InOrderTraverse(AVL_left(avl), function);
        function(avl);
        InOrderTraverse(AVL_right(avl), function);
    }
}

void PostOrderTraverse(const struct AVLImpl *avl, void(*function)(const struct AVLImpl *))
{
    if (avl)
    {
        PostOrderTraverse(AVL_left(avl), function);
        PostOrderTraverse(AVL_right(avl), function);
        function(avl);
    }
}

void print(const struct AVLImpl *avl, int space = 128, int wid = 4)
{
    std::queue<const struct AVLImpl *> q, w;
    q.push(avl);
    bool b = true;
    while (b)
    {
        b = false;
        while (!q.empty())
        {
            if (space - wid / 2 > 0) std::cout << std::string(space - wid / 2, ' ');
            if (q.front())
            {
                w.push(AVL_left(q.front()));
                w.push(AVL_right(q.front()));
                b = b || AVL_left(q.front()) || AVL_right(q.front());
                std::cout << std::to_string(AVL_getValue(q.front())) << std::setw(2) << AVL_getBf(q.front());
            }
            else
            {
                w.push(nullptr);
                w.push(nullptr);
                if (wid > 0) std::cout << std::string(wid, ' ');
            }
            if (space - wid / 2 > 0) std::cout << std::string(space - wid / 2, ' ');
            q.pop();
        }
        std::cout << "\n";
        space /= 2;
        q = std::move(w);
        w = std::queue<const struct AVLImpl *>{};
    }
}

void printNode(const struct AVLImpl *avl)
{
    std::cout << std::to_string(AVL_getValue(avl));
    std::cout << " ";
}

void printAVL(AVL *avl)
{
    std::cout << ":";
    InOrderTraverse(*avl, printNode);
}

int AVLTest()
{
    std::default_random_engine engine{ std::random_device{}() };
    std::uniform_int_distribution<int> random_int{
        10, //std::numeric_limits<int>::min(),
        99//std::numeric_limits<int>::max()
    };

    AVL_COMPARE = [](int lhs, int rhs) -> bool {
        return lhs > rhs;
    };

    std::set<int, decltype(AVL_COMPARE)> standard(AVL_COMPARE);
    AVL *avl = AVL_Construct();

    auto random_element = [&]() -> std::set<int>::iterator {
        if (standard.empty())
        {
            return standard.end();
        }
        std::uniform_int_distribution<size_t> random_adv{
            0,
            standard.size() - 1
        };
        auto n = random_adv(engine);
        auto it = standard.begin();
        if (n != 0) std::advance(it, n);
        return it;
    };

    auto empty = [&]() -> bool {
//        printAVL(avl);
        std::cout << "\n";

        std::cout << "empty();\n\n";

        return standard.empty() == AVL_empty(avl);
    };

    auto lower = [&](int n = 0) -> bool {
        if (!standard.empty())
        {
            if (n == 0)
            {
                n = *random_element();
                if (standard.find(n) == standard.end())
                {
                    return empty();
                }
            }

            printAVL(avl);
            std::cout << "\n";

            std::cout << "lower(";
            std::cout << std::to_string(n);
            std::cout << ");\n\n";

            auto it = standard.lower_bound(n);

            auto impl = AVL_lower_bound(avl, n);
            if ((impl == nullptr) != (it == standard.end()))
            {
                std::cout << std::to_string(it == standard.end());
                std::cout << std::to_string(impl == nullptr);
                std::cout << "\n";
                return false;
            }
            if (impl == nullptr || AVL_getValue(impl) == *it) return true;
            std::cout << "ERROR:\n";
            std::cout << "Expect:\t" + std::to_string(*it) + "\n";
            std::cout << "Actual:\t" + std::to_string(AVL_getValue(impl)) + "\n";
            std::cout << "\n";

            return false;
        }
        return empty();
    };

    auto upper = [&](int n = 0) -> bool {
        if (!standard.empty())
        {
            if (n == 0)
            {
                n = *random_element();
                if (standard.find(n) == standard.end())
                {
                    return empty();
                }
            }

            printAVL(avl);
            std::cout << "\n";

            std::cout << "upper(";
            std::cout << std::to_string(n);
            std::cout << ");\n\n";

            auto it = standard.upper_bound(n);

            auto impl = AVL_upper_bound(avl, n);
            if ((impl == nullptr) != (it == standard.end()))
            {
                std::cout << std::to_string(it == standard.end());
                std::cout << std::to_string(impl == nullptr);
                std::cout << "\n";
                return false;
            }
            if (impl == nullptr || AVL_getValue(impl) == *it) return true;
            std::cout << "ERROR:\n";
            std::cout << "Expect:\t" + std::to_string(*it) + "\n";
            std::cout << "Actual:\t" + std::to_string(AVL_getValue(impl)) + "\n";
            std::cout << "\n";

            return false;
        }
        return empty();
    };

    auto insert = [&](int r = 0) -> bool {
        if (r == 0) r = random_int(engine);
        if (standard.find(r) == standard.end())
        {
            printAVL(avl);
            std::cout << "\n";

            std::cout << "insert(";
            std::cout << std::to_string(r);
            std::cout << ");\n\n";

            standard.insert(r);
            AVL_Insert(avl, r);
        }
        return empty();
    };

    auto erase = [&](int r = 0) -> bool {
        if (standard.empty()) return empty();

        if (r == 0)
        {
            r = *random_element();
            auto it = standard.find(r);
            if (it == standard.end()) return empty();
        }

        printAVL(avl);

        std::cout << "\n";

        std::cout << "erase(";
        std::cout << std::to_string(r);
        std::cout << ");\n\n";

        standard.erase(standard.find(r));
        auto impl = AVL_lower_bound(avl, r);
        if (AVL_getValue(impl) != r) return false;
        AVL_Erase(avl, impl);
        return true;
    };

    std::vector<std::function<bool()>> actions{
        insert,
        insert,
        insert,
        erase,
        lower,
        upper,
        empty
    };

    std::uniform_int_distribution<size_t> random_action{
        0,
        actions.size() - 1
    };

//    insert(3);
//    insert(1);
//    insert(5);
//    insert(2);
//    insert(4);
//    insert(6);
//    insert(7);
//    print(*avl);
//    erase(4);
//    print(*avl);
//    insert(282);
//    insert(410);
//    insert(843);
//    insert(895);
//    insert(397);
//    erase(397);
//    insert(31);
//    erase(31);
//    insert(859);
//    insert(334);
//    insert(153);
//    erase(326);
//    erase(282);
//    insert(554);
//    isLegal(*avl);

//     printAVL(avl);
    std::cout << avl_log << std::endl;
    std::cout << "----------------------" << std::endl;

    auto test = [&]() -> bool {
        for (int i = 0; i < 10000; ++i)
        {
            auto result = actions[random_action(engine)]();
            // auto s = standard.size();
            print(*avl);
            if (!isLegal(*avl) || !result)
            {
                return false;
            }
        }
        return true;
    };

    bool result = test();
    if (result)
    {
        printAVL(avl);
        std::cout << "\n";
        std::cout << "all test success";
        std::cout << avl_log << std::endl;
    }
    else
    {
        printAVL(avl);
        std::cout << "\n";
        std::cout << "fail";
        std::cout << avl_log << std::endl;
    }

    AVL_Destruct(avl);
    return 0;
}
