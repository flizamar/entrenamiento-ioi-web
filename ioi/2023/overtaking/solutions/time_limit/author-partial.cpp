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
#include <variant>
#include <iterator>
#include <limits>
#include <iostream>

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

using packet = std::variant<car, sketch>;

long long get_start_time(const packet &x)
{
    return std::visit([](const auto &alt)
                      {
        if constexpr (std::is_same_v<std::decay_t<decltype(alt)>, car>) {
            return alt.curr;
        } else if constexpr (std::is_same_v<std::decay_t<decltype(alt)>, sketch>) {
            return alt.curr_first;
        } else {
            static_assert(!std::is_same_v<decltype(alt), decltype(alt)>);
        } },
                      x);
}

long long get_end_time(const packet &x)
{
    return std::visit([](const auto &alt)
                      {
        if constexpr (std::is_same_v<std::decay_t<decltype(alt)>, car>) {
            return alt.curr;
        } else if constexpr (std::is_same_v<std::decay_t<decltype(alt)>, sketch>) {
            return alt.curr_last;
        } else {
            static_assert(!std::is_same_v<decltype(alt), decltype(alt)>);
        } },
                      x);
}

void set_time(packet &x, long long time)
{
    std::visit([time](auto &alt)
               {
        if constexpr (std::is_same_v<std::decay_t<decltype(alt)>, car>) {
            alt.curr = time;
        } else if constexpr (std::is_same_v<std::decay_t<decltype(alt)>, sketch>) {
            alt.curr_first = alt.curr_last = time;
        } else {
            static_assert(!std::is_same_v<decltype(alt), decltype(alt)>);
        } },
               x);
}

void adjust(packet &x, long long len, long long X)
{
    std::visit([len, X](auto &alt)
               {
        if constexpr (std::is_same_v<std::decay_t<decltype(alt)>, car>) {
            alt.curr += static_cast<long long>(len) * alt.pace;
        } else if constexpr (std::is_same_v<std::decay_t<decltype(alt)>, sketch>) {
            alt.curr_first += static_cast<long long>(len) * X;
            alt.curr_last += static_cast<long long>(len) * X;
        } else {
            static_assert(!std::is_same_v<decltype(alt), decltype(alt)>);
        } },
               x);
}
long long get_end_time_adjusted(const packet &x, long long len, long long X)
{
    return std::visit([len, X](const auto &alt)
                      {
        if constexpr (std::is_same_v<std::decay_t<decltype(alt)>, car>) {
            return alt.curr + static_cast<long long>(len) * alt.pace;
        } else if constexpr (std::is_same_v<std::decay_t<decltype(alt)>, sketch>) {
            return alt.curr_last + static_cast<long long>(len) * X;
        } else {
            static_assert(!std::is_same_v<decltype(alt), decltype(alt)>);
        } },
                      x);
}

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
    long long lasttime = -1;
    for (long long i : indices)
    {
        // Ignore any cars at least as fast as my car, because they will never block me
        if (w[i] <= X)
            continue;
        if (lasttime != t[i])
        {
            packets.push_back({sketch{lasttime, t[i], lasttime, t[i]}});
            lasttime = t[i];
        }
        packets.push_back({car{t[i], w[i]}});
    }
    if (lasttime != MAX_START_TIME)
    {
        packets.push_back({sketch{lasttime, MAX_START_TIME, lasttime, MAX_START_TIME}});
    }
    p.push_back(L);
    for (long long j = 0; j <= P; ++j)
    {
        const long long len = p[j] - (j == 0 ? 0 : p[j - 1]);
        auto it = packets.begin();
        std::vector<packet> newpackets;
        auto out_it = std::back_inserter(newpackets);
        while (it != packets.end())
        {
            adjust(*it, len, X);
            long long curr_time = get_start_time(*it);
            if (curr_time != get_end_time(*it))
            {
                *out_it++ = *it++;
                continue;
            }
            long long min_sketch = std::numeric_limits<long long>::max();
            long long max_sketch = 0;
            auto it2 = it;
            if (std::holds_alternative<sketch>(*it2))
            {
                min_sketch = std::min(min_sketch, std::get<sketch>(*it2).orig_first);
                max_sketch = std::get<sketch>(*it2).orig_last;
            }
            for (++it2; it2 != packets.end() && get_end_time_adjusted(*it2, len, X) <= curr_time; ++it2)
            {
                adjust(*it2, len, X);
                if (std::holds_alternative<sketch>(*it2))
                {
                    min_sketch = std::min(min_sketch, std::get<sketch>(*it2).orig_first);
                    max_sketch = std::get<sketch>(*it2).orig_last;
                }
                else
                {
                    std::get<car>(*it2).curr = curr_time;
                }
            }
            if (it2 != packets.end() && std::holds_alternative<sketch>(*it2) && std::get<sketch>(*it2).curr_first + static_cast<long long>(len) * X < curr_time)
            {
                min_sketch = std::min(min_sketch, std::get<sketch>(*it2).orig_first);
                max_sketch = std::get<sketch>(*it2).orig_first += curr_time - (std::get<sketch>(*it2).curr_first + static_cast<long long>(len) * X);
                std::get<sketch>(*it2).curr_first = curr_time - static_cast<long long>(len) * X;
            }
            if (min_sketch <= max_sketch)
            {
                *out_it++ = sketch{min_sketch, max_sketch, curr_time, curr_time};
            }
            auto it3 = std::remove_if(it, it2, [](const packet &x)
                                      { return std::holds_alternative<sketch>(x); });
            std::sort(it, it3, [](const packet &a, const packet &b)
                      { return std::get<car>(a).pace < std::get<car>(b).pace; });
            out_it = std::move(it, it3, out_it);
            it = it2;
        }
        packets = std::move(newpackets);
    }
    for (packet &mPacket : packets)
    {
        if (std::holds_alternative<sketch>(mPacket))
        {
            query_sketches.push_back(std::get<sketch>(mPacket));
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
