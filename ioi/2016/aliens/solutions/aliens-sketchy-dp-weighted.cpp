const int ALLOWED_OPERATIONS = 4e7;

#include <bits/stdc++.h>
using namespace std;

#define jjs(i, s, x) for (int i = (s); i < int(x); ++i)
#define jjl(i, x) jjs(i, 0, x)
#define ji(x) jjl(i, x)
#define jj(x) jjl(j, x)
#define jk(x) jjl(k, x)
#define jij(a, b) ji(a) jj(b)
#define jij2d(v) jij((v).size(), (v)[i].size())
#define jjdescent(i, s, e) for (int i = (s)-1; i >= int(e); --i)
#define jjrev(i, s) jjdescent(i, s, 0)
#define foreach(x, C) for (auto& x : (C))
#define INF ((int) 1e9+10)
#define LINF ((long long) 1e16)
#define pb push_back
#define mp make_pair

#define rint readInteger
template<typename T>
bool readInteger(T& x)
{
	char c, r = 0, n = 0;
	x = 0;
	while (true)
	{
		c = getchar();
		if (c < 0 && !r)
			return 0;
		else if (c == '-' && !r)
			n = 1;
		else if (c >= '0' && c <= '9')
			x = x * 10 + c - '0', r = 1;
		else if (r)
			break;
	}
	if (n)
		x = -x;
	return 1;
}

template<typename T>
vector<T> readIntegral(int n)
{
	vector<T> ret(n);
	for (int i = 0; i < n; i++)
		readInteger(ret[i]);
	return ret;
}

auto readInts = readIntegral<int>;
auto readLongs = readIntegral<long long>;

template<typename T>
vector<vector<T>> make2d(size_t r, size_t c)
{
	return vector<vector<T>>(r, vector<T>(c));
}

template<typename T>
vector<vector<T>> make2d(size_t r, size_t c, const T& def)
{
	return vector<vector<T>>(r, vector<T>(c, def));
}

template <typename T, T MOD>
struct ModInt
{
	T value;
	ModInt() : value(0)
	{}
	ModInt(T x)
	{
		x %= MOD;
		if (x < 0)
			x += MOD;
		value = x;
	}

public:
	ModInt& operator += (ModInt x)
	{
		value += x.value;
		if (value >= MOD)
			value -= MOD;
		return *this;
	}
	ModInt& operator -= (ModInt x)
	{
		value -= x.value;
		if (value < 0)
			value += MOD;
		return *this;
	}
	ModInt& operator *= (ModInt x)
	{
		value *= x.value;
		value %= MOD;
		return *this;
	}
	ModInt& operator /= (ModInt x)
	{
		x.invert();
		return *this *= x;
	}

	ModInt operator + (ModInt x) const
	{
		ModInt o = *this;
		o += x;
		return o;
	}
	ModInt operator - (ModInt x) const
	{
		ModInt o = *this;
		o -= x;
		return o;
	}
	ModInt operator * (ModInt x) const
	{
		ModInt o = *this;
		o *= x;
		return o;
	}
	ModInt operator / (ModInt x) const
	{
		ModInt o = *this;
		o /= x;
		return o;
	}
	bool operator == (ModInt x) const
	{
		return value == x.value;
	}
	bool operator != (ModInt x) const
	{
		return !(*this == x);
	}

	ModInt operator - () const
	{
		return ModInt(0) - *this;
	}

	ModInt operator ^ (long long x) const
	{
		ModInt ret = 1 % MOD;
		ModInt mul = *this;
		while (x)
		{
			if (x & 1)
				ret *= mul;
			mul *= mul;
			x >>= 1;
		}
		return ret;
	}
	ModInt& operator ^= (long long x)
	{
		return *this = *this ^ x;
	}

private:
	void invert()
	{
		*this ^= MOD-2;
	}
public:
	friend ostream& operator << (ostream& out, const ModInt& x)
	{
		return out << x;
	}
};

template<typename T>
using v2d = vector<vector<T>>;

typedef ModInt<long long, 1000000007> mint;
typedef long long ll;
typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef v2d<int> VVI;
typedef vector<ll> VLL;
typedef v2d<ll> VVLL;
typedef vector<char> VCH;
typedef v2d<char> VVCH;

PII expand(PII a, PII b)
{
	return {min(a.first, b.first), max(a.second, b.second)};
}

PII contract(PII a, PII b)
{
	return {max(a.first, b.first), min(a.second, b.second)};
}

ll area(PII x) {
	ll z = x.second - x.first + 1;
	if (z <= 0)
		return 0;
	else
		return z * z;
}

ll gain(PII a, PII b)
{
	return area(expand(a, b)) - area(a) - area(b) + area(contract(a, b));
}

ll intersect(PII a, PII b)
{
	return area(contract(a, b));
}

struct entry {
	int x;
	ll base;

	ll cost(int y) {
		ll d = y - x + 1;
		assert(d >= 0);
		return d * d + base;
	}
};

ll sqr(ll x) {
	return x * x;
}

double meet(const entry& a, const entry& b)
{
	ll c1 = a.base + sqr(a.x);
	ll c2 = b.base + sqr(b.x);
	return (double) (c2 - c1) / (-2 * (a.x - b.x));
}

struct hull {
	deque<entry> q;

	ll bestCost(int x) {
		while (q.size() > 1) {
			if (q[0].cost(x) > q[1].cost(x))
				q.pop_front();
			else
				break;
		}
		if (q.empty())
			return LINF;
		else
			return q[0].cost(x);
	}

	void offer(entry e, int y)
	{
		while (q.size() > 1 && meet(e, q.back()) <= meet(q.back(), q[q.size()-2])) {
			q.pop_back();
		}
		while (q.size() > 0 && q.back().cost(y) >= e.cost(y)) {
			q.pop_back();
		}
		q.push_back(e);
	}
};

ll solve(int sz, int photos, VPII pts)
{
	int n = pts.size();
	vector<hull> hulls(photos);
	int begin = pts[0].first;
	int end = pts.back().second;
	ji(photos) hulls[i].offer({begin, 0LL}, begin);
	int margin = n;
	while ((ll) photos * margin > ALLOWED_OPERATIONS) {
		--margin;
	}
	VI weight(n);
	ji(n) weight[i] = 1;
	for (int i = 1; i < n; i++) {
		weight[i] = 1 + pts[i].first - pts[i-1].first;
	}
	ll weightTot = 0;
	foreach(x, weight)
		weightTot += x;
	ll weightSum = 0;
	int idx = 0;
	VI balanceA(photos);
	VI balanceB(photos);
	ji(photos) {
		while (idx < n-1 && weightSum < weightTot * i / photos) {
			weightSum += weight[idx];
			++idx;
		}
		balanceA[i] = max(0, idx - margin / 2);
		balanceB[i] = min(n-1, balanceA[i] + margin);
		balanceA[i] = max(0, balanceB[i] - margin);
	}
	int a = 1;
	int b = 0;
	for (int j = 1; j < n; j++) {
		while (b < photos-1 && balanceA[b+1] <= j)
			++b;
		while (a <= b && balanceB[a] < j)
			++a;
		for (int i = b; i >= a; i--) {
			hulls[i].offer({pts[j].first, hulls[i-1].bestCost(pts[j-1].second) - intersect(pts[j-1], pts[j])}, pts[j].second);
		}
	}
	return hulls.back().bestCost(end);
}

long long take_photos(int n, int m, int k, vector<int> row, vector<int> col) {
	VPII ranges;
	ji(row.size()) {
		int a = min(row[i], col[i]);
		int b = max(row[i], col[i]);
		ranges.pb({a, b});
	}
	foreach(x, ranges) x.second *= -1;
	sort(ranges.begin(), ranges.end());
	foreach(x, ranges) x.second *= -1;
	VPII nranges;
	foreach(r, ranges) {
		if (nranges.empty() || r.second > nranges.back().second)
			nranges.pb(r);
	}
	return solve(m, k, nranges);
}

// BEGIN CUT

int main() {
    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);
    std::vector<int> r(n), c(n);
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &r[i], &c[i]);
    }
    long long ans = take_photos(n, m, k, r, c);
    
    // BEGIN SECRET
    puts("098d134608c94f7413faac591054ee35");
    // END SECRET
    
    printf("%lld\n", ans);
    return 0;
}

// END CUT
