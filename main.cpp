#include <iostream>

#include "AVLtest.h"
#include "sortTest.h"
#include "AVL.h"

int main()
{
    AVL_COMPARE = [](int lhs, int rhs) -> bool {
        return lhs > rhs;
    };

    std::cout << "Hello, World!" << std::endl;
    AVLTest();
    return 0;
}
