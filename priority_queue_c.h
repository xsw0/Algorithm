//
// Created by 0x0 on 6/20/21.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ALGORITHM_PRIORITY_QUEUE_C_H
#define ALGORITHM_PRIORITY_QUEUE_C_H

#include <stddef.h>

typedef int PRIORITY_QUEUE_VALUE_TYPE;
typedef struct
{
    PRIORITY_QUEUE_VALUE_TYPE* arr;
    size_t size;
    size_t capacity;
} priority_queue;

PRIORITY_QUEUE_VALUE_TYPE extract_min(priority_queue* pq);

void make_heap(priority_queue* pq, const PRIORITY_QUEUE_VALUE_TYPE s[], size_t n);

#endif //ALGORITHM_PRIORITY_QUEUE_C_H

#ifdef __cplusplus
}
#endif
