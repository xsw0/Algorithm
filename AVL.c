//
// Created by 0x0 on 4/29/21.
//

#include "AVL.h"

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

struct AVLImpl
{
    struct AVLImpl *parent;
    struct AVLImpl *left;
    struct AVLImpl *right;
    int bf;
    AVL_VALUE_TYPE value;
};

bool Compare(AVL_VALUE_TYPE lhs, AVL_VALUE_TYPE rhs)
{
    return lhs < rhs;
}

bool AVL_empty(AVL *avl)
{
    assert(avl);
    return *avl == NULL;
}

AVL_VALUE_TYPE AVL_getValue(const struct AVLImpl *impl)
{
    return impl->value;
}

const struct AVLImpl *AVL_left(const struct AVLImpl *impl)
{
    assert(impl);
    return impl->left;
}

const struct AVLImpl *AVL_right(const struct AVLImpl *impl)
{
    assert(impl);
    return impl->right;
}

const struct AVLImpl *AVL_lower_bound(AVL *avl, AVL_VALUE_TYPE value)
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

const struct AVLImpl *AVL_upper_bound(AVL *avl, AVL_VALUE_TYPE value)
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

// return the new root
static struct AVLImpl *leftRotate(struct AVLImpl *root)
{
    assert(root);

    struct AVLImpl *pivot = root->right;
    assert(pivot);

    struct AVLImpl *mid = pivot->left;

    if (root->parent)
    {
        struct AVLImpl *parent = root->parent;
        if (root == root->parent->left)
        {
            parent->left = pivot;
            if (abs(root->bf - 1) < abs(root->bf))
            {
                ++parent->bf;
            }
            else if (abs(root->bf - 1) > abs(root->bf))
            {
                --parent->bf;
            }
        }
        else
        {
            assert(root == root->parent->right);
            parent->right = pivot;
            if (abs(root->bf - 1) < abs(root->bf))
            {
                --parent->bf;
            }
            else if (abs(root->bf - 1) > abs(root->bf))
            {
                ++parent->bf;
            }
        }
        pivot->parent = parent;
    }

    pivot->left = root;
    root->parent = pivot;

    root->right = mid;
    if (mid) mid->parent = root;

    --root->bf;
    if (pivot->bf > 0) root->bf -= pivot->bf;

    --pivot->bf;
    if (root->bf < 0) pivot->bf += root->bf;

    return pivot;
}

// return the new root
static struct AVLImpl *rightRotate(struct AVLImpl *root)
{
    assert(root);

    struct AVLImpl *pivot = root->right;
    assert(pivot);

    struct AVLImpl *mid = pivot->left;

    if (root->parent)
    {
        struct AVLImpl *parent = root->parent;
        if (root == root->parent->left)
        {
            parent->left = pivot;
            if (abs(root->bf + 1) < abs(root->bf))
            {
                ++parent->bf;
            }
            else if (abs(root->bf + 1) > abs(root->bf))
            {
                --parent->bf;
            }
        }
        else
        {
            assert(root == root->parent->right);
            parent->right = pivot;
            if (abs(root->bf + 1) < abs(root->bf))
            {
                --parent->bf;
            }
            else if (abs(root->bf + 1) > abs(root->bf))
            {
                ++parent->bf;
            }
        }
        pivot->parent = parent;
    }

    pivot->right = root;
    root->parent = pivot;

    root->left = mid;
    if (mid) mid->parent = root;

    ++root->bf;
    if (pivot->bf < 0) root->bf -= pivot->bf;

    ++pivot->bf;
    if (root->bf > 0) pivot->bf += root->bf;

    return pivot;
}

static void rebalance(struct AVLImpl *impl)
{
    assert(impl);
    if (impl->bf > 1)
    {
        assert(impl->right);
        if (impl->right->bf < 0) rightRotate(impl->right);
        leftRotate(impl);
        if (impl->parent) rebalance(impl->parent);
    }
    else if (impl->bf < -1)
    {
        assert(impl->left);
        if (impl->left->bf > 0) leftRotate(impl->left);
        rightRotate(impl);
        if (impl->parent) rebalance(impl->parent);
    }
}

static void AVLImpl_Destruct(struct AVLImpl *impl)
{
    assert(impl);
    if (impl->left) AVLImpl_Destruct(impl->left);
    if (impl->right) AVLImpl_Destruct(impl->right);
    free(impl);
}

void AVL_erase(AVL *avl, struct AVLImpl *impl)
{
    assert(impl);
    struct AVLImpl *parent = impl->parent;
    struct AVLImpl **parent_child = NULL;
    int _bf = 0;
    if (parent)
    {
        assert(parent->left == impl || parent->right == impl);
        _bf = parent->left == impl ? -1 : 1;
        parent_child = parent->left == impl ? &parent->left : &parent->right;
    }

    if (!impl->left && !impl->right)
    {
        if (parent)
        {
            *parent_child = NULL;
            parent->bf -= _bf * 1;
            rebalance(parent);
        }
        else
        {
            *avl = NULL;
        }
        AVLImpl_Destruct(impl);
    }
    else if (!(impl->left && impl->right))
    {
        struct AVLImpl *pivot = impl->left ? impl->left : impl->right;
        if (parent)
        {
            *parent_child = pivot;
            pivot->parent = parent;
            parent->bf -= _bf * 1;
            rebalance(parent);
        }
        else
        {
            *avl = pivot;
            pivot->parent = NULL;
        }
        AVLImpl_Destruct(impl);
    }
    else
    {
        if (impl->bf < 0)
        {
            struct AVLImpl *predecessor = impl->left;
            while (predecessor->right) predecessor = predecessor->right;
            impl->value = predecessor->value;
            AVL_erase(avl, predecessor);
        }
        else
        {
            struct AVLImpl *successor = impl->right;
            while (successor->left) successor = successor->left;
            impl->value = successor->value;
            AVL_erase(avl, successor);
        }
    }
}

const struct AVLImpl *AVL_insert(AVL *avl, AVL_VALUE_TYPE value)
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
                    --impl->bf;
                    rebalance(impl);
                    break;
                }
            }
            else
            {
                assert(Compare(impl->value, value));    // Prohibit same data insertion
                if (impl->right)
                {
                    impl = impl->right;
                }
                else
                {
                    impl->right = node;
                    node->parent = impl;
                    ++impl->bf;
                    rebalance(impl);
                    break;
                }
            }
        }
    }
    else
    {
        node->parent = NULL;
        *avl = node;
    }
    return node;
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
