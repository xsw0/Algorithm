//
// Created by 0x0 on 4/22/21.
//

#ifndef ALGORITHM__PRIORITY_QUEUE_H_
#define ALGORITHM__PRIORITY_QUEUE_H_

#include <vector>

template<class Tp,
    class Container = std::vector<Tp>,
    class Compare = std::less<typename Container::value_type>>
class priority_queue
{
    Container heap;
    Compare compare;

    typename Container::iterator left(typename Container::iterator it)
    {
        auto dis = std::distance(heap.begin(), it) * 2 + 1;
        return dis < heap.size() ? heap.begin() + dis : heap.end();
    };
    typename Container::iterator right(typename Container::iterator it)
    {
        auto dis = std::distance(heap.begin(), it) * 2 + 2;
        return dis < heap.size() ? heap.begin() + dis : heap.end();
    };
    typename Container::iterator parent(typename Container::iterator it)
    {
        if (it == heap.begin()) return heap.end();
        auto dis = std::distance(heap.begin(), prev(it)) / 2;
        return heap.begin() + dis;
    };

    void bubble_up(typename Container::iterator it)
    {
        auto p = this->parent(it);
        if (p != heap.end() && compare(*it, *p))
        {
            using std::swap;
            swap(*it, *p);
            bubble_up(p);
        }
    }

    void bubble_down(typename Container::iterator it)
    {
        auto minIt = it;
        auto l = left(it);
        auto r = right(it);
        if (l != heap.end() && compare(*l, *minIt)) minIt = l;
        if (r != heap.end() && compare(*r, *minIt)) minIt = r;
        if (minIt != it)
        {
            using std::swap;
            swap(*it, *minIt);
            bubble_down(minIt);
        }
    }

 public:
    explicit priority_queue(Compare compare = Compare()) : compare(compare) {};

    void reserve(size_t size)
    {
        heap.reserve(size);
    }

//    void insert(const Tp &element)
//    {
//        heap.push_back(element);
//        if (heap.begin() != std::prev(heap.end()))
//        {
//            bubble_up(std::prev(heap.end()));
//        }
//    }

    template<class ForwardIterator>
    void emplace(ForwardIterator first, ForwardIterator last)
    {
        heap = std::move(Container(first, last));
        for (auto it = heap.begin() + heap.size() / 2; it != heap.begin(); --it)
        {
            bubble_down(it);
        }
        bubble_down(heap.begin());
    }

    Tp extract_min()
    {
        assert(!heap.empty());
        auto result = heap.front();
        heap.front() = heap.back();
        heap.pop_back();
        if (!heap.empty())
        {
            bubble_down(heap.begin());
        }
        return result;
    }
};

#endif //ALGORITHM__PRIORITY_QUEUE_H_
