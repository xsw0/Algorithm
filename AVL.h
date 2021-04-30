//
// Created by 0x0 on 4/29/21.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ALGORITHM__AVL_H_
#define ALGORITHM__AVL_H_

#include <stdbool.h>

typedef int AVL_VALUE_TYPE;

struct AVLImpl
{
    struct AVLImpl *parent;
    struct AVLImpl *left;
    struct AVLImpl *right;
    int bf;
    AVL_VALUE_TYPE value;
};

typedef struct AVLImpl *AVL;

bool Compare(AVL_VALUE_TYPE lhs, AVL_VALUE_TYPE rhs);

AVL *AVL_Construct();
void AVL_Destruct(AVL *avl);

bool AVL_empty(AVL *avl);

struct AVLImpl *AVL_lower_bound(AVL *avl, AVL_VALUE_TYPE value);
struct AVLImpl *AVL_upper_bound(AVL *avl, AVL_VALUE_TYPE value);

void AVL_insert(AVL *avl, AVL_VALUE_TYPE value);
void AVL_erase(AVL *avl, AVL_VALUE_TYPE value);

#endif //ALGORITHM__AVL_H_

#ifdef __cplusplus
}
#endif
