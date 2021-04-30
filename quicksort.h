//
// Created by 0x0 on 4/23/21.
//

#ifndef ALGORITHM__QUICKSORT_H_
#define ALGORITHM__QUICKSORT_H_

#include <random>

template<class Compare, class RandomAccessIterator>
void quicksort_impl(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    using std::swap;

//    static std::default_random_engine re(std::random_device{}());

    if (first != last)
    {
        auto pivot = first;
        auto lm1 = std::prev(last);

//        swap(*lm1, *(first + std::uniform_int_distribution<int>(0, std::distance(first, last) - 1)(re)));

        for (auto it = first; it != last; ++it)
        {
            if (comp(*it, *lm1))
            {
                swap(*it, *pivot);
                ++pivot;
            }
        }
        swap(*pivot, *lm1);
        quicksort(first, pivot, comp);
        quicksort(std::next(pivot), last, comp);
    }
}

// This forwarder keeps the top call and the recursive calls using the same instantiation, forcing a reference Compare
template<class RandomAccessIterator, class Compare>
inline void quicksort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    using Comp_ref = typename std::__comp_ref_type<Compare>::type;
    quicksort_impl<Comp_ref>(first, last, Comp_ref(comp));
}

template<class RandomAccessIterator>
inline void quicksort(RandomAccessIterator first, RandomAccessIterator last)
{
    quicksort(first, last, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

template<class Tp>
inline void quicksort(Tp **first, Tp **last)
{
    quicksort((size_t *)first, (size_t *)last, std::less<>());
}

template<class Tp>
inline void quicksort(std::__wrap_iter<Tp *> first, std::__wrap_iter<Tp *> last)
{
    quicksort(first.base(), last.base());
}

template<class Tp, class Compare>
inline void quicksort(std::__wrap_iter<Tp *> first, std::__wrap_iter<Tp *> last, Compare comp)
{
    using Comp_ref = typename std::add_lvalue_reference<Compare>::type;
    quicksort<Tp *, Comp_ref>(first.base(), last.base(), comp);
}

// https://www.zhihu.com/question/24189847/answer/130319417
template<class Compare, class RandomAccessIterator>
void quick_sort_tail_impl(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    using std::swap;

//    static std::default_random_engine re(std::random_device{}());

    while (first != last)
    {
        auto pivot = first;
        auto lm1 = std::prev(last);

//        swap(*lm1, *(first + std::uniform_int_distribution<int>(0, std::distance(first, last) - 1)(re)));

        for (auto it = first; it != last; ++it)
        {
            if (comp(*it, *lm1))
            {
                swap(*it, *pivot);
                ++pivot;
            }
        }
        swap(*pivot, *lm1);

        quick_sort_tail_impl(first, pivot, comp);
        first = std::next(pivot);
    }
}

// This forwarder keeps the top call and the recursive calls using the same instantiation, forcing a reference Compare
template<class RandomAccessIterator, class Compare>
inline void quick_sort_tail(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    using Comp_ref = typename std::__comp_ref_type<Compare>::type;
    quick_sort_tail_impl<Comp_ref>(first, last, Comp_ref(comp));
}

template<class RandomAccessIterator>
inline void quick_sort_tail(RandomAccessIterator first, RandomAccessIterator last)
{
    quick_sort_tail(first, last, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

template<class Tp>
inline void quick_sort_tail(Tp **first, Tp **last)
{
    quick_sort_tail((size_t *)first, (size_t *)last, std::less<>());
}

template<class Tp>
inline void quick_sort_tail(std::__wrap_iter<Tp *> first, std::__wrap_iter<Tp *> last)
{
    quick_sort_tail(first.base(), last.base());
}

template<class Tp, class Compare>
inline void quick_sort_tail(std::__wrap_iter<Tp *> first, std::__wrap_iter<Tp *> last, Compare comp)
{
    using Comp_ref = typename std::add_lvalue_reference<Compare>::type;
    quick_sort_tail<Tp *, Comp_ref>(first.base(), last.base(), comp);
}

#endif //ALGORITHM__QUICKSORT_H_
