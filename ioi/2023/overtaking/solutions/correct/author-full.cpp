// This is the full solution.
//
// O(N * P * log(N) + Q * log(N * P))
//
// Can be optimised to O(N * (P + log (N)) + Q * log(N * P)) if the sorting is done carefully,
// by noticing that every pair of cars can only swap positions at most once.
// But this seems kind of difficult to implement,
// and it is likely that the time taken will be hard to distinguish the slower solution from the faster solution.

#include "overtaking.h"

#include <algorithm>
#include <utility>
#include <vector>

// Linked list node for the singly linked list.
template <typename T>
struct singly_node
{
    T val;
    singly_node *next;
};

// Singly linked list that has polong longers to both the front and back.
template <typename T>
struct singly_list
{
private:
    singly_node<T> *first;
    singly_node<T> *last;

public:
    singly_list() : first(nullptr), last(nullptr) {}
    singly_list(singly_list &&other) : first(std::exchange(other.first, nullptr)), last(std::exchange(other.last, nullptr)) {}
    singly_list &operator=(singly_list &&other)
    {
        first = std::exchange(other.first, nullptr);
        last = std::exchange(other.last, nullptr);
        return *this;
    }
    bool empty() const { return first == nullptr; }
    void push_back(singly_node<T> *node)
    {
        node->next = nullptr;
        if (empty())
        {
            first = last = node;
        }
        else
        {
            last->next = node;
            last = node;
        }
    }
    void append(singly_list &&other)
    {
        if (!other.empty())
        {
            if (!empty())
            {
                last->next = std::exchange(other.first, nullptr);
                last = std::exchange(other.last, nullptr);
            }
            else
            {
                first = std::exchange(other.first, nullptr);
                last = std::exchange(other.last, nullptr);
            }
        }
    }
    singly_node<T> *pop_front()
    {
        singly_node<T> *res = std::exchange(first, first->next);
        if (first == nullptr)
            last = nullptr;
        return res;
    }
    singly_node<T> *front() const
    {
        return first;
    }
};

// Represents a car (not my car) that is travelling on the road.
struct car
{
    // The time that this car reaches the current passing place.
    long long curr;
    // The pace that this car drives at.
    long long pace;
};

// Represents what will happen if my car starts at the starting line between orig_first and orig_last.
struct sketch
{
    // The range of times at the starting line that this sketch represents.
    long long orig_first;
    long long orig_last;

    // The range of times at the current passing place that this sketch represents.  This is stored as the time at the starting line in order to get to the current passing place at the correct time.
    // If curr_first != curr_last, then curr_last - curr_first == orig_last - orig_first, meaning that this range of cars have not been forced to slow down due to a car in front yet.
    // Otherwise, curr_first == curr_last, meaning that the range of original times are now compressed long longo a single polong long in time.
    long long curr_first;
    long long curr_last;
};

struct packet
{
    car mCar;
    singly_list<sketch> sketches;
};

constexpr long long MAX_START_TIME = 1'000'000'000'000'000'000;

std::vector<sketch> query_sketches;

void init(int L, int N, std::vector<long long> t, std::vector<int> w, int X, int P, std::vector<int> p2)
{
    // Ignore the number of queries; we don't care about it
    //(void) Q;

    P -= 2;
    std::vector<int> p(P);
    for (int i = 0; i < P; i++)
        p[i] = p2[i + 1];

    std::vector<singly_node<sketch>> list_buf;
    list_buf.reserve((N + 1) * (P + 2));

    // This is the state that we update as we travel down the road
    std::vector<packet> packets;

    std::vector<long long> indices;
    indices.reserve(N);
    for (long long i = 0; i < N; ++i)
    {
        indices.push_back(i);
    }
    std::sort(indices.begin(), indices.end(), [&](long long a, long long b)
              {
        if (t[a] != t[b]) return t[a] < t[b];
        return w[a] < w[b]; });
    packets.push_back({car{-1, X}, {}});
    for (long long i : indices)
    {
        // Ignore any cars at least as fast as my car, because they will never block me
        if (w[i] <= X)
            continue;
        if (packets.back().mCar.curr != t[i])
        {
            singly_node<sketch> &node = list_buf.emplace_back();
            node.val = sketch{static_cast<long long>(packets.back().mCar.curr), t[i], packets.back().mCar.curr, t[i]};
            packets.back().sketches.push_back(&node);
        }
        packets.push_back({car{t[i], w[i]}, {}});
    }
    if (packets.back().mCar.curr != MAX_START_TIME)
    {
        singly_node<sketch> &node = list_buf.emplace_back();
        node.val = sketch{static_cast<long long>(packets.back().mCar.curr), MAX_START_TIME, packets.back().mCar.curr, MAX_START_TIME};
        packets.back().sketches.push_back(&node);
    }
    p.push_back(L);
    for (long long j = 0; j <= P; ++j)
    {
        const long long len = p[j] - (j == 0 ? 0 : p[j - 1]);
        auto it = packets.begin();
        while (it != packets.end())
        {
            singly_list<sketch> collect = std::move(it->sketches);
            const long long curr_time = it->mCar.curr += it->mCar.pace * len;
            auto it2 = it;
            for (++it2; it2 != packets.end() && curr_time >= it2->mCar.curr + it2->mCar.pace * len; ++it2)
            {
                it2->mCar.curr = curr_time;
                collect.append(std::move(it2->sketches));
            }
            // To get the better time complexity (see comment at the top of this file),
            // you need to notice that every pair of cars cannot change relative order more than once,
            // and then change this sorting algorithm to something that is faster when there are subranges that are already sorted.
            // This is probably not going to be distinguishable from the current solution by time limit allowances.
            std::sort(it, it2, [](const packet &a, const packet &b)
                      { return a.mCar.pace < b.mCar.pace; });
            const long long delta = static_cast<long long>(X) * p[j];
            if (!collect.empty() && (collect.front()->val.curr_first + delta < curr_time || collect.front()->val.curr_last + delta <= curr_time))
            {
                const long long orig_first = collect.front()->val.orig_first;
                long long orig_last = orig_first;
                while (!collect.empty() && collect.front()->val.curr_last + delta <= curr_time)
                {
                    orig_last = collect.pop_front()->val.orig_last;
                }
                if (!collect.empty() && collect.front()->val.curr_first + delta < curr_time)
                {
                    orig_last = collect.front()->val.orig_first += (curr_time - delta) - collect.front()->val.curr_first;
                    collect.front()->val.curr_first = curr_time - delta;
                }
                singly_node<sketch> &node = list_buf.emplace_back();
                node.val = sketch{orig_first, orig_last, curr_time - delta, curr_time - delta};
                (it - 1)->sketches.push_back(&node);
            }
            (it2 - 1)->sketches.append(std::move(collect));
            it = it2;
        }
    }
    const long long delta = static_cast<long long>(X) * L;
    for (packet &packet : packets)
    {
        singly_list<sketch> &sketches = packet.sketches;
        while (!sketches.empty())
        {
            query_sketches.push_back(sketches.pop_front()->val);
            query_sketches.back().curr_first += delta;
            query_sketches.back().curr_last += delta;
        }
    }
}

long long arrival_time(long long q)
{
    auto it = std::lower_bound(query_sketches.begin(), query_sketches.end(), q, [](const sketch &s, long long q2)
                               { return s.orig_last < q2; });
    if (it->curr_first == it->curr_last)
    {
        return it->curr_first;
    }
    return it->curr_first + (q - it->orig_first);
}
