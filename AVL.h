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

struct AVLImpl;

typedef struct AVLImpl *AVL;

bool Compare(AVL_VALUE_TYPE lhs, AVL_VALUE_TYPE rhs);

AVL *AVL_Construct();
void AVL_Destruct(AVL *avl);

bool AVL_empty(AVL *avl);

const struct AVLImpl *AVL_lower_bound(AVL *avl, AVL_VALUE_TYPE value);
const struct AVLImpl *AVL_upper_bound(AVL *avl, AVL_VALUE_TYPE value);

const struct AVLImpl *AVL_insert(AVL *avl, AVL_VALUE_TYPE value);
void AVL_erase(AVL *avl, struct AVLImpl *impl);

AVL_VALUE_TYPE AVL_getValue(const struct AVLImpl *impl);

const struct AVLImpl *AVL_left(const struct AVLImpl *impl);
const struct AVLImpl *AVL_right(const struct AVLImpl *impl);

#endif //ALGORITHM__AVL_H_

#ifdef __cplusplus
}
#endif
