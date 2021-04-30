//
// Created by 0x0 on 4/22/21.
//

#ifndef ALGORITHM____HEAPSORT_H_
#define ALGORITHM____HEAPSORT_H_

#include "priority_queue.h"

template<class Compare, class Tp>
void heapsort_impl(Tp *first, Tp *last, Compare comp)
{
    priority_queue<Tp,
                   std::vector<Tp>,
                   Compare> pq(comp);
    pq.emplace(first, last);
    while (first != last)
    {
        *first = pq.extract_min();
        ++first;
    }
}

template<class Compare, class RandomAccessIterator>
void heapsort_impl(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    priority_queue<typename RandomAccessIterator::value_type,
                   std::vector<typename RandomAccessIterator::value_type>,
                   Compare> pq(comp);
    pq.emplace(first, last);
    while (first != last)
    {
        *first = pq.extract_min();
        ++first;
    }
}

// This forwarder keeps the top call and the recursive calls using the same instantiation, forcing a reference Compare
template<class RandomAccessIterator, class Compare>
inline void heapsort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    using Comp_ref = typename std::__comp_ref_type<Compare>::type;
    heapsort_impl<Comp_ref>(first, last, Comp_ref(comp));
}

template<class RandomAccessIterator>
inline void heapsort(RandomAccessIterator first, RandomAccessIterator last)
{
    heapsort(first, last, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

template<class Tp>
inline void heapsort(Tp **first, Tp **last)
{
    heapsort((size_t *)first, (size_t *)last, std::less<>());
}

template<class Tp>
inline void heapsort(std::__wrap_iter<Tp *> first, std::__wrap_iter<Tp *> last)
{
    heapsort(first.base(), last.base());
}

template<class Tp, class Compare>
inline void heapsort(std::__wrap_iter<Tp *> first, std::__wrap_iter<Tp *> last, Compare comp)
{
    using Comp_ref = typename std::add_lvalue_reference<Compare>::type;
    heapsort<Tp *, Comp_ref>(first.base(), last.base(), comp);
}

#endif //ALGORITHM____HEAPSORT_H_
