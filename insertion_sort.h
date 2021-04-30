//
// Created by 0x0 on 4/25/21.
//

#ifndef ALGORITHM__INSERTION_SORT_H_
#define ALGORITHM__INSERTION_SORT_H_

template<class Compare, class RandomAccessIterator>
void insertion_sort_impl(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    using std::swap;
    if (first != last)
    {
        for (auto it = std::next(first); it != last; ++it)
        {
            RandomAccessIterator l = it;
            RandomAccessIterator r;
            while (l != first)
            {
                r = l;
                --l;
                if (comp(*r, *l))
                {
                    swap(*l, *r);
                }
            }
        }
    }
}

// This forwarder keeps the top call and the recursive calls using the same instantiation, forcing a reference Compare
template<class RandomAccessIterator, class Compare>
inline void insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    using Comp_ref = typename std::__comp_ref_type<Compare>::type;
    insertion_sort_impl<Comp_ref>(first, last, Comp_ref(comp));
}

template<class RandomAccessIterator>
inline void insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    insertion_sort(first, last, std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>());
}

template<class Tp>
inline void insertion_sort(Tp **first, Tp **last)
{
    insertion_sort((size_t *)first, (size_t *)last, std::less<>());
}

template<class Tp>
inline void insertion_sort(std::__wrap_iter<Tp *> first, std::__wrap_iter<Tp *> last)
{
    insertion_sort(first.base(), last.base());
}

template<class Tp, class Compare>
inline void insertion_sort(std::__wrap_iter<Tp *> first, std::__wrap_iter<Tp *> last, Compare comp)
{
    using Comp_ref = typename std::add_lvalue_reference<Compare>::type;
    insertion_sort<Tp *, Comp_ref>(first.base(), last.base(), comp);
}

#endif //ALGORITHM__INSERTION_SORT_H_
