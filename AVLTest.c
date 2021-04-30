//
// Created by 0x0 on 4/30/21.
//

#include "AVLtest.h"

#include <stdio.h>
#include <stdlib.h>

#include "AVL.h"

void PreOrderTraverse(struct AVLImpl *avl, void(*function)(struct AVLImpl *))
{
    if (avl)
    {
        function(avl);
        PreOrderTraverse(avl->left, function);
        PreOrderTraverse(avl->right, function);
    }
}

void InOrderTraverse(struct AVLImpl *avl, void(*function)(struct AVLImpl *))
{
    if (avl)
    {
        InOrderTraverse(avl->left, function);
        function(avl);
        InOrderTraverse(avl->right, function);
    }
}

void PostOrderTraverse(struct AVLImpl *avl, void(*function)(struct AVLImpl *))
{
    if (avl)
    {
        PostOrderTraverse(avl->left, function);
        PostOrderTraverse(avl->right, function);
        function(avl);
    }
}

void printNode(struct AVLImpl *avl)
{
    printf("%d ", avl->value);
}

int AVLTest()
{
    AVL *avl = AVL_Construct();

    for (int i = 0; i < 50; ++i)
    {
        int n = abs(rand()) % 100;
        AVL_insert(avl, n);
    }

//    PreOrderTraverse(*avl, printNode);
//    printf("\n");
    InOrderTraverse(*avl, printNode);
    printf("\n");
//    PostOrderTraverse(*avl, printNode);
//    printf("\n");

    int n;
    while (scanf("%d", &n) != EOF)
    {
        struct AVLImpl *lower = AVL_lower_bound(avl, n);
        struct AVLImpl *upper = AVL_upper_bound(avl, n);
        if (lower) { printf("%d ", AVL_lower_bound(avl, n)->value); }
        else { printf("NULL "); }
        if (upper) { printf("%d ", AVL_upper_bound(avl, n)->value); }
        else { printf("NULL "); }
        printf("\n");
    }

    AVL_Destruct(avl);
    return 0;
}
