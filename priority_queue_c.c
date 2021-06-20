//
// Created by 0x0 on 6/20/21.
//

#include "priority_queue_c.h"

#include <stdlib.h>
#include <assert.h>

static size_t pq_parent(size_t n)
{
    assert(n != 0);
    return (n - 1) / 2;
}

static size_t left(size_t n)
{
    return n * 2 + 1;
}

static size_t right(size_t n)
{
    return n * 2 + 2;
}

static void pq_swap(priority_queue* pq, size_t i, size_t j)
{
    PRIORITY_QUEUE_VALUE_TYPE temp;
    temp = pq->arr[i];
    pq->arr[i] = pq->arr[j];
    pq->arr[j] = temp;
}

static void bubble_down(priority_queue* pq, size_t n)
{
    size_t lower = n;
    if (left(n) < pq->size && pq->arr[left(n)] < pq->arr[n]) lower = left(n);
    if (right(n) < pq->size && pq->arr[right(n)] < pq->arr[n]) lower = right(n);
    if (lower != n)
    {
        pq_swap(pq, lower, n);
        bubble_down(pq, lower);
    }
}

PRIORITY_QUEUE_VALUE_TYPE extract_min(priority_queue* pq)
{
    assert(pq->size != 0);

    PRIORITY_QUEUE_VALUE_TYPE result = pq->arr[0];

    --pq->size;
    pq_swap(pq, 0, pq->size);
    bubble_down(pq, 0);

    return pq->arr[pq->size];
}

void make_heap(priority_queue* pq, const PRIORITY_QUEUE_VALUE_TYPE s[], size_t n)
{
    pq->capacity = n;
    pq->arr = realloc(pq->arr, pq->capacity * sizeof(PRIORITY_QUEUE_VALUE_TYPE));
    pq->size = n;

    for (size_t i = 0; i < n; ++i) pq->arr[i] = s[i];
    while (n-- > 0) bubble_down(pq, n);
}
