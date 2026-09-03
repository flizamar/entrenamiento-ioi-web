#include "longesttrip.h"
#include <bits/stdc++.h>
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

bool sajat = 0;
/*
const int maxNumberOfCalls = 32640;
const int maxTotalNumberOfCalls = 150000;
const int maxTotalNumberOfLandmarksInCalls = 1500000;
int call_counter = 0;
int total_call_counter = 0;
int landmark_counter = 0;

int C, N, D;
std::vector<std::vector<int>> U;

static inline void protocol_violation(std::string message) {
    printf("Protocol Violation: %s\n", message.c_str());
    exit(0);
}

bool are_connected(std::vector<int> A, std::vector<int> B) {
    if (sajat) {
        cout << "nagy kerdes:\n";
        for (auto x:A) {
            cout << x << " ";
        }
        cout << "\n";
        for (auto y:B) {
            cout << y << " ";
        }
        cout << "\n";
        int x;
        cin >> x;
        return x;
    }
    ++call_counter; ++ total_call_counter;
    if (call_counter > maxNumberOfCalls || total_call_counter > maxTotalNumberOfCalls) {
        protocol_violation("too many calls");
    }

    int nA = A.size(), nB = B.size();
    landmark_counter += nA + nB;
    if (landmark_counter > maxTotalNumberOfLandmarksInCalls) {
        protocol_violation("too many elements");
    }

    if (nA == 0 || nB == 0) {
        protocol_violation("invalid array");
    }
    for (int i = 0; i < nA; ++i) {
        if (A[i] < 0 || N <= A[i]) {
            protocol_violation("invalid array");
        }
        for (int j = 0; j < i; ++j) {
            if (A[i] == A[j]) {
                protocol_violation("invalid array");
            }
        }
    }
    for (int i = 0; i < nB; ++i) {
        if (B[i] < 0 || N <= B[i]) {
            protocol_violation("invalid array");
        }
        for (int j = 0; j < i; ++j) {
            if (B[i] == B[j]) {
                protocol_violation("invalid array");
            }
        }
    }

    for (int i = 0; i < nA; ++i) {
        for (int j = 0; j < nB; ++j) {
            if (A[i] == B[j]){
                protocol_violation("non-disjoint arrays");
            }
        }
    }

    for (int i = 0; i < nA; ++i) {
        for (int j = 0; j < nB; ++j) {
            if (U[std::max(A[i],B[j])][std::min(A[i],B[j])] == 1) {
                return true;
            }
        }
    }

    return false;
}

*/
bool el(int x, int y)
{
    if (sajat)
    {
        cout << "kerdes: " << x << " " << y << "\n";
        int res;
        cin >> res;
        return res;
    }
    return are_connected({x}, {y});
}

std::vector<int> longest_trip(int N, int D)
{
    int n = N;
    vector<int> sor;
    for (int i = 1; i < N; i++)
    {
        sor.push_back(i);
    }
    random_shuffle(sor.begin(), sor.end());
    vector<int> a, b;
    a.push_back(0);
    bool spec = 0;
    for (int j = 1; j < N; j++)
    {
        int i = sor[j - 1];
        if (el(i, a.back()))
        {
            a.push_back(i);
            spec = 0;
        }
        else
        {
            if (b.size() == 0 || spec || el(i, b.back()))
            {
                b.push_back(i);
                spec = 1;
            }
            else
            {
                if (b.size() == 1)
                {
                    spec = 1;
                }
                else
                {
                    spec = 0;
                }
                reverse(b.begin(), b.end());
                for (auto x : b)
                {
                    a.push_back(x);
                }
                b.clear();
                b.push_back(i);
            }
        }
        if (sajat)
        {
            cout << "vege:\n";
            for (auto x : a)
            {
                cout << x << " ";
            }
            cout << "\n";
            for (auto x : b)
            {
                cout << x << " ";
            }
            cout << "\n";
            cout << "spec: " << spec << "\n";
        }
    }
    int sa = a.size(), sb = b.size();
    /*for (int i=1; i<sa; i++) {
        assert(el(a[i-1], a[i]));
    }
    for (int i=1; i<sb; i++) {
        assert(el(b[i-1], b[i]));
    }*/
    if (b.size() > a.size())
    {
        swap(a, b);
    }

    if (a.size() > 0 && b.size() > 0)
    {
        if (el(a.back(), b.back()))
        {
            reverse(b.begin(), b.end());
            for (auto x : b)
            {
                a.push_back(x);
            }
            return a;
        }
        else if (el(a[0], b.back()))
        {
            vector<int> c;
            for (auto x : b)
            {
                c.push_back(x);
            }
            for (auto x : a)
            {
                c.push_back(x);
            }
            /*assert(c.size()==n);
            for (int i=1; i<n; i++) {
                assert(el(c[i-1], c[i]));
            }*/
            return c;
        }
        else if (el(a[0], b[0]))
        {
            vector<int> c;
            reverse(b.begin(), b.end());
            for (auto x : b)
            {
                c.push_back(x);
            }
            for (auto x : a)
            {
                c.push_back(x);
            }
            return c;
        }
        else if (el(a.back(), b[0]))
        {
            vector<int> c;
            for (auto x : a)
            {
                c.push_back(x);
            }
            for (auto x : b)
            {
                c.push_back(x);
            }
            return c;
        }
        else
        {
            if (!are_connected(a, b))
            {
                return a;
            }
            int lo = 0, hi = a.size(), mid;
            while (hi - lo > 1)
            {
                mid = (hi + lo) / 2;
                vector<int> aa;
                for (int i = 0; i < mid; i++)
                {
                    aa.push_back(a[i]);
                }
                if (are_connected(aa, b))
                {
                    hi = mid;
                }
                else
                {
                    lo = mid;
                }
            }
            int pa = lo;
            vector<int> ha = {a[lo]};
            lo = 0, hi = b.size(), mid = 0;
            while (hi - lo > 1)
            {
                mid = (hi + lo) / 2;
                vector<int> bb;
                for (int i = 0; i < mid; i++)
                {
                    bb.push_back(b[i]);
                }
                if (are_connected(ha, bb))
                {
                    hi = mid;
                }
                else
                {
                    lo = mid;
                }
            }
            int pb = lo;

            int sa = a.size(), sb = b.size();
            vector<int> c;
            for (int i = pa + 1; i <= pa + sa; i++)
            {
                c.push_back(a[i % sa]);
            }
            for (int i = pb; i < pb + sb; i++)
            {
                c.push_back(b[i % sb]);
            }
            return c;
        }
    }
    else
    {
        return a;
    }
}

/*
int main() {
    if (sajat) {
        int n;
        cin >> n;
        vector<int> res=longest_trip(n, 1);
        for (auto x:res) {
            cout << x << " ";
        }
        return 0;
    }
    assert(1 == scanf("%d", &C));
    int maximumCalls = 0;
    for (int c = 0; c < C; ++c) {
        call_counter = 0;
        assert(2 == scanf("%d %d", &N, &D));

        U.resize(N);
        for (int i = 1; i < N; ++i) {
            U[i].resize(i);
            for (int j = 0; j < i; ++j) {
                assert(1 == scanf("%d", &U[i][j]));
            }
        }

        for (int i = 2; i < N; ++i) {
            for (int j = 1; j < i; ++j) {
                for (int k = 0; k < j; ++k) {
                    if (U[i][j] + U[i][k] + U[j][k] < D) {
                        printf("Insufficient Density\n");
                        exit(0);
                    }
                }
            }
        }

        std::vector<int> T = longest_trip(N, D);
        int l = T.size();
        cout << "valasz:           ";
        for (int i = 0; i < l; ++i) {
            printf(i == 0 ? "%d" : " %d", T[i]);
        }
        printf("\n");
        printf("%d\n", call_counter);

        maximumCalls = std::max(maximumCalls, call_counter);
        call_counter = 0;
    }
    printf("%d\n", maximumCalls);

    return 0;
}
*/
/*
2
5 1
1
1 1
0 0 1
0 0 0 1
4 1
1
0 0
0 0 1

*/
