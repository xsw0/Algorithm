//
// Created by 0x0 on 4/30/21.
//

#include "AVLtest.h"

#include <stdio.h>
#include <stdlib.h>

#include "AVL.h"

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
    printf("%d ", AVL_getValue(avl));
}

void insert(AVL *avl, AVL_VALUE_TYPE arr[], size_t arrSize)
{
    for (int i = 0; i < arrSize; ++i)
    {
        AVL_insert(avl, arr[i]);
    }
}

int AVLTest(int arr[], unsigned long long arrSize)
{
    AVL *avl = AVL_Construct();

    insert(avl, arr, arrSize);

//    PreOrderTraverse(*avl, printNode);
//    printf("\n");
    InOrderTraverse(*avl, printNode);
    printf("\n");
//    PostOrderTraverse(*avl, printNode);
//    printf("\n");

    int n;
    while (scanf("%d", &n) != EOF)
    {
        const struct AVLImpl *lower = AVL_lower_bound(avl, n);
        const struct AVLImpl *upper = AVL_upper_bound(avl, n);

        if (lower)
        {
            printf("%d ", AVL_getValue(AVL_lower_bound(avl, n)));
        }
        else
        {
            printf("NULL ");
        }

        if (upper)
        {
            printf("%d ", AVL_getValue(AVL_upper_bound(avl, n)));
        }
        else
        {
            printf("NULL ");
        }

        printf("\n");
    }

    AVL_Destruct(avl);
    return 0;
}
