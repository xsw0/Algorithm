//
// Created by 0x0 on 4/29/21.
//

#include "AVL.h"

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

bool Compare(AVL_VALUE_TYPE lhs, AVL_VALUE_TYPE rhs)
{
    return lhs < rhs;
}

bool AVL_empty(AVL *avl)
{
    assert(avl);
    return *avl == NULL;
}

struct AVLImpl *AVL_lower_bound(AVL *avl, AVL_VALUE_TYPE value)
{
    struct AVLImpl *impl = *avl;
    struct AVLImpl *result = NULL;
    while (impl)
    {
        if (Compare(impl->value, value))
        {
            if (impl->right)
            {
                impl = impl->right;
            }
            else
            {
                return result;
            }
        }
        else
        {
            result = impl;
            if (impl->left)
            {
                impl = impl->left;
            }
            else
            {
                return result;
            }
        }
    }
    return result;
}

struct AVLImpl *AVL_upper_bound(AVL *avl, AVL_VALUE_TYPE value)
{
    struct AVLImpl *impl = *avl;
    struct AVLImpl *result = NULL;
    while (impl)
    {
        if (Compare(value, impl->value))
        {
            result = impl;
            if (impl->left)
            {
                impl = impl->left;
            }
            else
            {
                return result;
            }
        }
        else
        {
            if (impl->right)
            {
                impl = impl->right;
            }
            else
            {
                return result;
            }
        }
    }
    return result;
}

static void leftRotate(struct AVLImpl *impl)
{
    assert(impl);
    assert(impl->right);

    struct AVLImpl *l = impl->right;
    struct AVLImpl *r = impl->right->right;
    struct AVLImpl *ll = impl->left;
    struct AVLImpl *lr = impl->right->left;

    AVL_VALUE_TYPE t = l->value;
    l->value = impl->value;
    impl->value = t;

    impl->left = l;
    impl->right = r;

    l->parent = impl;
    l->left = ll;
    l->right = lr;

    if (ll) ll->parent = l;
    if (lr) lr->parent = l;

    int pivotBf = l->bf;
    int rootBf = impl->bf;

    l->bf = rootBf - 1;
    if (pivotBf > 0) l->bf -= pivotBf;

    impl->bf = pivotBf - 1;
    if (l->bf < 0) impl->bf += l->bf;
}

static void rightRotate(struct AVLImpl *impl)
{
    assert(impl);
    assert(impl->left);

    struct AVLImpl *l = impl->left->left;
    struct AVLImpl *r = impl->left;
    struct AVLImpl *rl = impl->left->right;
    struct AVLImpl *rr = impl->right;

    AVL_VALUE_TYPE t = r->value;
    r->value = impl->value;
    impl->value = t;

    impl->left = l;
    impl->right = r;

    r->parent = impl;
    r->left = rl;
    r->right = rr;

    if (rl) rl->parent = r;
    if (rr) rr->parent = r;

    int pivotBf = r->bf;
    int rootBf = impl->bf;

    r->bf = rootBf + 1;
    if (pivotBf < 0) r->bf -= pivotBf;

    impl->bf = pivotBf + 1;
    if (r->bf > 0) impl->bf += r->bf;
}

static void rebalance(struct AVLImpl *impl)
{
    assert(impl);

    switch (impl->bf)
    {
    case 0:
    case -1:
    case 1:
        if (impl->parent != NULL)
        {
            if (impl == impl->parent->left)
            {
                impl->parent->bf -= 1;
            }

            if (impl == impl->parent->right)
            {
                impl->parent->bf += 1;
            }

            if (impl->parent->bf != 0)
            {
                rebalance(impl->parent);
            }
        }
        break;
    default:
        if (impl->bf > 0)
        {
            if (impl->right && impl->right->bf < 0)
            {
                rightRotate(impl->right);
                leftRotate(impl);
            }
            else
            {
                leftRotate(impl);
            }
        }
        else if (impl->bf < 0)
        {
            if (impl->left && impl->left->bf > 0)
            {
                leftRotate(impl->left);
                rightRotate(impl);
            }
            else
            {
                rightRotate(impl);
            }
        }
        break;
    }
}

static void AVLImpl_Destruct(struct AVLImpl *impl)
{
    assert(impl);
    if (impl->left) AVLImpl_Destruct(impl->left);
    if (impl->right) AVLImpl_Destruct(impl->right);
    free(impl);
}

void AVL_erase(AVL *avl, AVL_VALUE_TYPE value)
{
    assert(avl);
    struct AVLImpl *impl = *avl;
    while (impl)
    {
        if (Compare(value, impl->value))
        {
            if (impl->left)
            {
                impl = impl->left;
            }
            else
            {
                assert(0);
            }
        }
        else if (Compare(impl->value, value))
        {
            if (impl->right)
            {
                impl = impl->right;
            }
            else
            {
                assert(0);
            }
        }
        else
        {
            rebalance(impl);
            AVLImpl_Destruct(impl);
        }
    }
}

void AVL_insert(AVL *avl, AVL_VALUE_TYPE value)
{
    assert(avl);
    struct AVLImpl *impl = *avl;

    struct AVLImpl *node = malloc(sizeof(struct AVLImpl));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->bf = 0;

    if (impl)
    {
        while (impl)
        {
            if (Compare(value, impl->value))
            {
                if (impl->left)
                {
                    impl = impl->left;
                }
                else
                {
                    impl->left = node;
                    node->parent = impl;
                    break;
                }
            }
            else if (Compare(impl->value, value))
            {
                if (impl->right)
                {
                    impl = impl->right;
                }
                else
                {
                    impl->right = node;
                    node->parent = impl;
                    break;
                }
            }
            else
            {
                // for rand test
//                assert(0);
                return;
            }
        }
    }
    else
    {
        node->parent = NULL;
        *avl = node;
    }

    rebalance(node);
}

AVL *AVL_Construct()
{
    AVL *avl = malloc(sizeof(AVL));
    *avl = NULL;
    return avl;
}

void AVL_Destruct(AVL *avl)
{
    assert(avl);
    if (*avl) AVLImpl_Destruct(*avl);
    free(avl);
}
