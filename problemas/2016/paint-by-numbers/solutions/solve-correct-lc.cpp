#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<int> load_vector() {
    int size; cin >> size;
    vector<int> tmp(size); for (int &x : tmp) cin >> x;
    return tmp;
}

vector< vector<bool> > get_possible_prefixes(const vector<int> &C, const vector<int> &B, const vector<int> &W) {
    int L = B.size(), N = C.size();

    vector<int> Wsums(L+1,0); for (int l=0; l<L; ++l) Wsums[l+1]=Wsums[l]+W[l];

    vector< vector<bool> > possible( L+1, vector<bool>(N+1, false) );

    possible[0][0] = true;
    for (int l=1; l<=L; ++l) possible[l][0] = possible[l-1][0] && !B[l-1];

    for (int n=1; n<=N; ++n) {
        for (int l=1; l<=L; ++l) {
            // may cell l-1 be white?
            if (!B[l-1] && possible[l-1][n]) possible[l][n] = true;
            // may cell l-1 be black -- i.e., the end of block n-1?
            if (!W[l-1]) {
                int len = C[n-1];
                if (l < len) continue; // doesn't fit
                if (Wsums[l] - Wsums[l-len] > 0) continue; // overlaps a forced-white cell
                if (l > len && B[l-len-1]) continue; // touches a forced-black cell on the left end
                if (possible[max(0,l-len-1)][n-1]) possible[l][n] = true;
            }
        }
    }
    return possible;
}

std::string solve_puzzle(std::string S, std::vector<int> C) {
    int L = S.size();
    int N = C.size();

    vector<int> B(L,0); for (int i = 0; i < L; i++) B[i] = S[i] == 'X';
    vector<int> W(L,0); for (int i = 0; i < L; i++) W[i] = S[i] == '_';

    vector< vector<bool> > can_fit_prefix = get_possible_prefixes(C,B,W);
    reverse( C.begin(), C.end() ); reverse( B.begin(), B.end() ); reverse( W.begin(), W.end() );
    
    vector< vector<bool> > can_fit_suffix = get_possible_prefixes(C,B,W);
    reverse( C.begin(), C.end() ); reverse( B.begin(), B.end() ); reverse( W.begin(), W.end() ); 

    vector<bool> can_be_white(L,false);
    for (int l=0; l<L; ++l) for (int n=0; n<=N; ++n) 
        if (!B[l] && can_fit_prefix[l][n] && can_fit_suffix[L-l-1][N-n]) can_be_white[l] = true;

    vector<int> Wsums(L+1,0); for (int l=0; l<L; ++l) Wsums[l+1]=Wsums[l]+W[l];

    vector<int> intervals(L+1,0);
    for (int n=0; n<N; ++n) {
        for (int start=0; start+C[n]<=L; ++start) {
            if (start > 0 && B[start-1]) continue;
            if (start+C[n]<L && B[start+C[n]]) continue;
            if (Wsums[start+C[n]] > Wsums[start]) continue;
            if (can_fit_prefix[max(0,start-1)][n] && can_fit_suffix[max(0,L-start-C[n]-1)][N-n-1]) {
                intervals[start] += 1;
                intervals[start+C[n]] -= 1;
            }
        }
    }

    vector<bool> can_be_black(L+1,false);
    int inside = 0;
    for (int l=0; l<L; ++l) {
        inside += intervals[l];
        if (inside > 0) can_be_black[l] = true;
    }

    std::string ans;
    for (int l=0; l<L; ++l) {
        if (can_be_black[l] && can_be_white[l]) ans.push_back('?');
        if (can_be_black[l] && !can_be_white[l]) ans.push_back('X');
        if (!can_be_black[l] && can_be_white[l]) ans.push_back('_');
        if (!can_be_black[l] && !can_be_white[l]) ans.push_back('!');
    }
    return ans;
}

const int S_MAX_LEN = 200 * 1000;
char buf[S_MAX_LEN + 1];

// BEGIN CUT

int main() {
    scanf("%s", buf);
    std::string s = buf;
    int c_len;
    scanf("%d", &c_len);
    std::vector<int> c(c_len);
    for (int i = 0; i < c_len; i++) {
        scanf("%d", &c[i]);
    }
    std::string ans = solve_puzzle(s, c);
    
    // BEGIN SECRET
    puts("098d134608c94f7413faac591054ee35");
    // END SECRET

    printf("%s\n", ans.data());

    
}

// END CUT
