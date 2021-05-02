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

bool (*AVL_COMPARE)(AVL_VALUE_TYPE lhs, AVL_VALUE_TYPE rhs);

int AVL_getBf(const struct AVLImpl *impl)
{
    return impl->bf;
}

struct tup *isLegal2(struct AVLImpl *root)
{
    assert(root);
    struct tup *t = malloc(sizeof(struct tup));
    t->min = root->value;
    t->max = root->value;
    struct tup *l = NULL;
    struct tup *r = NULL;
    int maxHeight = 0;

    if (root->left) l = isLegal2(root->left);
    if (l == NULL)
    {
        l = malloc(sizeof(struct tup));
        l->height = 0;
    }
    else
    {
        if (l->height > maxHeight) maxHeight = l->height;
        if (!AVL_COMPARE(l->max, root->value))
        {
            return NULL;
        }
        t->min = l->min;
    }

    if (root->right) r = isLegal2(root->right);
    if (r == NULL)
    {
        r = malloc(sizeof(struct tup));
        r->height = 0;
    }
    else
    {
        if (r->height > maxHeight) maxHeight = r->height;
        if (!AVL_COMPARE(root->value, r->min))
        {
            return NULL;
        }
        t->max = r->max;
    }

    t->height = 1 + maxHeight;
    if (root->bf != r->height - l->height)
    {
        return NULL;
    }
    free(l);
    free(r);
    return t;
}

bool isLegal(struct AVLImpl *root)
{
    if (root == NULL) return true;
    return isLegal2(root);
}

static struct AVLImpl *AVLImpl_Construct(struct AVLImpl *parent, AVL_VALUE_TYPE value)
{
    struct AVLImpl *impl = malloc(sizeof(struct AVLImpl));
    assert(impl);
    impl->parent = parent;
    impl->left = NULL;
    impl->right = NULL;
    impl->bf = 0;
    impl->value = value;
    return impl;
}

static void AVLImpl_Destruct(struct AVLImpl *impl)
{
    assert(impl);
    if (impl->left) AVLImpl_Destruct(impl->left);
    if (impl->right) AVLImpl_Destruct(impl->right);
    free(impl);
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
        if (AVL_COMPARE(impl->value, value))
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
        if (AVL_COMPARE(value, impl->value))
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

static void leftRotate(struct AVLImpl **p_root)
{
    struct AVLImpl *root = *p_root;
    assert(root);

    struct AVLImpl *pivot = root->right;
    assert(pivot);

    struct AVLImpl *mid = pivot->left;

    struct AVLImpl *parent = root->parent;
    if (root->parent)
    {
        assert(root == root->parent->left || root == root->parent->right);
        if (root == root->parent->left)
        {
            parent->left = pivot;
        }
        else
        {
            parent->right = pivot;
        }
    }
    pivot->parent = parent;

    pivot->left = root;
    root->parent = pivot;

    root->right = mid;
    if (mid) mid->parent = root;

    --root->bf;
    if (pivot->bf > 0) root->bf -= pivot->bf;

    --pivot->bf;
    if (root->bf < 0) pivot->bf += root->bf;

    *p_root = pivot;
}

static void rightRotate(struct AVLImpl **p_root)
{
    struct AVLImpl *root = *p_root;
    assert(root);

    struct AVLImpl *pivot = root->left;
    assert(pivot);

    struct AVLImpl *mid = pivot->right;

    struct AVLImpl *parent = root->parent;
    if (root->parent)
    {
        assert(root == root->parent->left || root == root->parent->right);
        if (root == root->parent->left)
        {
            parent->left = pivot;
        }
        else
        {
            parent->right = pivot;
        }
    }
    pivot->parent = parent;

    pivot->right = root;
    root->parent = pivot;

    root->left = mid;
    if (mid) mid->parent = root;

    ++root->bf;
    if (pivot->bf < 0) root->bf -= pivot->bf;

    ++pivot->bf;
    if (root->bf > 0) pivot->bf += root->bf;

    *p_root = pivot;
}

static struct AVLImpl *AVL_rebalance(struct AVLImpl *impl, int LR, int change)
{
    assert(change == -1 || change == 1);
    assert(impl->bf >= -1 && impl->bf <= 1);

    int bf = impl->bf;
    impl->bf += LR * change;

    if (impl->bf > 1)
    {
        assert(impl->right);
        if (impl->right->bf < 0) rightRotate(&impl->right);
        leftRotate(&impl);
    }
    else if (impl->bf < -1)
    {
        assert(impl->left);
        if (impl->left->bf > 0) leftRotate(&impl->left);
        rightRotate(&impl);
    }

    if (impl->parent)
    {
        struct AVLImpl *parent = impl->parent;
        assert(impl == parent->left || impl == parent->right);
        int lr = impl == parent->left ? -1 : 1;
        if (bf == 0 && change > 0)
        {
            AVL_rebalance(impl->parent, lr, 1);
        }
        else if (impl->bf == 0 && change < 0)
        {
            AVL_rebalance(impl->parent, lr, -1);
        }
    }
    return impl->parent;
}

const struct AVLImpl *AVL_Insert(AVL *avl, AVL_VALUE_TYPE value)
{
    assert(avl);
    if (*avl == NULL)
    {
        *avl = AVLImpl_Construct(NULL, value);
        return *avl;
    }

    struct AVLImpl *impl = *avl;

    while (impl)
    {
        if (AVL_COMPARE(value, impl->value))
        {
            if (impl->left)
            {
                impl = impl->left;
            }
            else
            {
                impl->left = AVLImpl_Construct(impl, value);
                AVL_rebalance(impl, -1, 1);
                while ((*avl)->parent) *avl = (*avl)->parent;
                return impl->left;
            }
        }
        else if (AVL_COMPARE(impl->value, value))
        {
            if (impl->right)
            {
                impl = impl->right;
            }
            else
            {
                impl->right = AVLImpl_Construct(impl, value);
                AVL_rebalance(impl, 1, 1);
                while ((*avl)->parent) *avl = (*avl)->parent;
                return impl->left;
            }
        }
        else
        {
            return impl;
        }
    }
    assert(0);
}

void AVL_Erase(AVL *avl, const struct AVLImpl *impl)
{
    assert(impl);
    struct AVLImpl *non_const_impl = (struct AVLImpl *)impl;
    struct AVLImpl *parent = non_const_impl->parent;
    struct AVLImpl **parent_child = NULL;
    int LR;
    if (parent)
    {
        assert(parent->left == non_const_impl || parent->right == non_const_impl);
        LR = parent->left == non_const_impl ? -1 : 1;
        parent_child = parent->left == non_const_impl ? &parent->left : &parent->right;
    }

    if (!non_const_impl->left && !non_const_impl->right)
    {
        if (parent)
        {
            *parent_child = NULL;
            AVL_rebalance(parent, LR, -1);
            while ((*avl)->parent) *avl = (*avl)->parent;
        }
        else
        {
            *avl = NULL;
        }
        AVLImpl_Destruct(non_const_impl);
    }
    else if (!(non_const_impl->left && non_const_impl->right))
    {
        struct AVLImpl *pivot = non_const_impl->left ? non_const_impl->left : non_const_impl->right;
        if (parent)
        {
            *parent_child = pivot;
            pivot->parent = parent;
            AVL_rebalance(parent, LR, -1);
            while ((*avl)->parent) *avl = (*avl)->parent;
        }
        else
        {
            *avl = pivot;
            pivot->parent = NULL;
        }
        non_const_impl->left = NULL;
        non_const_impl->right = NULL;
        AVLImpl_Destruct(non_const_impl);
    }
    else
    {
        if (non_const_impl->bf < 0)
        {
            struct AVLImpl *predecessor = non_const_impl->left;
            while (predecessor->right) predecessor = predecessor->right;
            non_const_impl->value = predecessor->value;
            AVL_Erase(avl, predecessor);
        }
        else
        {
            struct AVLImpl *successor = non_const_impl->right;
            while (successor->left) successor = successor->left;
            non_const_impl->value = successor->value;
            AVL_Erase(avl, successor);
        }
    }
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
