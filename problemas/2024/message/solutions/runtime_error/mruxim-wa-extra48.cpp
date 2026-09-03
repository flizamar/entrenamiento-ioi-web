#include "message.h"
#include <bits/stdc++.h>

using namespace std;

#define rep(i, n) for(int i = 0, i##__n = (int)(n); i < i##__n; ++i)
#define fer(i, a, b) for(int i = (int)(a), i##__b = (int)(b); i < i##__b; ++i)
#define rof(i, b, a) for(int i = (int)(b), i##__a = (int)(a); i-- > i##__a; )
#define sz(x) (int((x).size()))
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define X first
#define Y second
//#define endl '\n'

template<class P, class Q> inline void smin(P &a, Q b) { if (b < a) a = b; }
template<class P, class Q> inline void smax(P &a, Q b) { if (a < b) a = b; }

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<double, double> pdd;

///////////////////////////////////////////////////////////////////////////

const int plen = 31;
const int bm = (1U << 31) - 1;
const int thr = 48;

int rndnext(int seed) { return (seed * 197845321 + 23527573) & bm; }

vector<int> calcX(int k) {
	vector<int> X(k);
	int seed = 76;
	rep(i, k) X[i] = seed = rndnext(seed);
	return X;
}

vector<bool> pack(int x) {
	vector<bool> res(plen);
	rep(i, plen) res[i] = (bool)(x >> i & 1);
	return res;
}

int unpack(vector<bool> v) {
	int res = 0;
	rep(i, plen) res |= ((int)v[i]) << i;
	return res;
}

int poof(int x, int mask) {
	int res = 0;
	for(; mask; mask &= mask-1, x >>= 1)
		res |= (x & 1) << __builtin_ctz(mask);
	return res;
}

int unpoof(int x, int mask) {
	int res = 0;
	for(int i = 0; mask; mask &= mask-1, i++)
		res |= (x >> __builtin_ctz(mask) & 1) << i;
	return res;
}

// int cnt(int i) { return i < thr ? 15 : 16; }

void send_message(vector<bool> M, vector<bool> C) {
	rep(i, plen) C[i] = !C[i];
	int n = sz(M);
	int k = max(thr, (n + 15) / 16 + 3);
	vector<int> X = calcX(k);

	int cmask = unpack(C);
	int id = __builtin_ctz(cmask);

	vector<int> A(k, 0);
	rep(i, plen) A[i] |= (cmask >> i & 1) << id;
	int rem = k * 16 - thr - n;
	rep(i, 4) A[plen + i] |= ((min(rem, 15)) >> i & 1) << id;
//	 i=[plen+4..thr) : (A[i] >> id) & 1 == 0

	int pos = 0;
	rep(i, k) rep(b, plen) if(pos < n && (cmask >> b & 1) && (i >= thr || b != id))
		A[i] |= ((int)M[pos++]) << b;
	if(rem > 15) A.back() = poof(rem, cmask ^ (1<<id));

	rep(i, k) send_packet(pack(A[i] ^ X[i]));
}

vector<bool> receive_message(vector<vector<bool>> R) {
	int k = sz(R);
	vector<int> X = calcX(k);
	vector<int> A(k);
	rep(i, k) A[i] = unpack(R[i]) ^ X[i];

	int id = -1, cmask = -1, rem = -1;
	rep(b, plen) {
		int tcmask = 0; rep(i, plen) tcmask |= (A[i] >> b & 1) << i;
		int t = 0; fer(i, plen + 4, thr) t |= (A[i] >> b & 1) << (i - plen - 4);
		if(__builtin_popcount(tcmask) != 16 || (tcmask >> b & 1) == 0 || t != 0) continue;
		int trem = 0; rep(i, 4) trem |= ((A[plen + i] >> b & 1) << i);
/*		int tmp = unpoof(A.back(), tcmask ^ (1<<b)) >> (15 - trem);
		if(trem != 15 && tmp != 0) continue;
		if(trem == 15 && tmp >= cnt(sz(A)-1) + cnt(sz(A)-2) && unpoof(A[sz(A)-2], tcmask ^ (1<<b)) > 0) continue;
*/		
		if(id != -1) assert(0); // unlucky!
		id = b, cmask = tcmask;
		rem = trem;
	}

	if(int tmp = unpoof(A.back(), cmask ^ (1<<id)); rem == 15 && tmp != 0) rem = tmp;
	rem = k * 16 - thr - rem;

	vector<bool> M;
	rep(i, k) rep(b, plen) if(rem > 0 && (cmask >> b & 1) && (i >= thr || b != id))
		M.push_back((bool)(A[i] >> b & 1)), rem--;

	return M;
}
