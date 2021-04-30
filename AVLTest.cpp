//
// Created by 0x0 on 4/30/21.
//

#include "AVLtest.h"

#include <iostream>
#include <set>
#include <random>
#include <string>
#include <vector>

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

void printNode(const struct AVLImpl *avl)
{
    avl_log += std::to_string(AVL_getValue(avl));
    avl_log += " ";
}

void printAVL(AVL *avl)
{
    avl_log += ":";
    InOrderTraverse(*avl, printNode);
}

int AVLTest()
{
    std::default_random_engine engine{ std::random_device{}() };
    std::uniform_int_distribution<int> random_int{
        1, //std::numeric_limits<int>::min(),
        999 //std::numeric_limits<int>::max()
    };

    std::set<int> standard;
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
        printAVL(avl);
        avl_log += "\n";

        avl_log += "empty();\n\n";

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
            avl_log += "\n";

            avl_log += "lower(";
            avl_log += std::to_string(n);
            avl_log += ");\n\n";

            auto it = standard.lower_bound(n);

            auto impl = AVL_lower_bound(avl, n);
            if ((impl == nullptr) != (it == standard.end()))
            {
                avl_log += std::to_string(it == standard.end());
                avl_log += std::to_string(impl == nullptr);
                avl_log += "\n";
                return false;
            }
            if (impl == nullptr || AVL_getValue(impl) == *it) return true;
            avl_log += "ERROR:\n";
            avl_log += "Expect:\t" + std::to_string(*it) + "\n";
            avl_log += "Actual:\t" + std::to_string(AVL_getValue(impl)) + "\n";
            avl_log += "\n";

            return false;
        }
        return true;
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
            avl_log += "\n";

            avl_log += "upper(";
            avl_log += std::to_string(n);
            avl_log += ");\n\n";

            auto it = standard.upper_bound(n);

            auto impl = AVL_upper_bound(avl, n);
            if ((impl == nullptr) != (it == standard.end()))
            {
                avl_log += std::to_string(it == standard.end());
                avl_log += std::to_string(impl == nullptr);
                avl_log += "\n";
                return false;
            }
            if (impl == nullptr || AVL_getValue(impl) == *it) return true;
            avl_log += "ERROR:\n";
            avl_log += "Expect:\t" + std::to_string(*it) + "\n";
            avl_log += "Actual:\t" + std::to_string(AVL_getValue(impl)) + "\n";
            avl_log += "\n";

            return false;
        }
        return true;
    };

    auto insert = [&](int r = 0) -> bool {
        if (r == 0) r = random_int(engine);
        if (standard.find(r) == standard.end())
        {
            printAVL(avl);
            avl_log += "\n";

            avl_log += "insert(";
            avl_log += std::to_string(r);
            avl_log += ");\n\n";

            standard.insert(r);
            AVL_Insert(avl, r);
        }
        return true;
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

        avl_log += "\n";

        avl_log += "erase(";
        avl_log += std::to_string(r);
        avl_log += ");\n\n";

        standard.erase(standard.find(r));
        auto impl = AVL_lower_bound(avl, r);
        if (AVL_getValue(impl) != r) return false;
        AVL_Erase(avl, impl);
        return true;
    };

    std::vector<std::function<bool()>> actions{
        insert,
//        insert,
//        insert,
        erase,
        lower,
        upper,
        empty
    };

    std::uniform_int_distribution<size_t> random_action{
        0,
        actions.size() - 1
    };




//     printAVL(avl);
    std::cout << avl_log << std::endl;
    std::cout << "----------------------" << std::endl;

    auto test = [&]() -> bool {
        for (int i = 0; i < 1000; ++i)
        {
            auto result = actions[random_action(engine)]();
            auto s = standard.size();
            isLegal(*avl);
            if (!result)
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
        avl_log += "\n";
        avl_log += "all test success";
        std::cout << avl_log << std::endl;
    }
    else
    {
        printAVL(avl);
        avl_log += "\n";
        avl_log += "fail";
        std::cout << avl_log << std::endl;
    }

    AVL_Destruct(avl);
    return 0;
}
