#include "hieroglyphs.h"
#include<bits/stdc++.h>


using namespace std;

using vi = vector<int>;
using vvi = vector<vi>;
using ii = pair<int, int>;

namespace tomek {
    

constexpr int alphabet = 200'001;

vector<int> _ucs(
    const vector<int> text_a,
    const vector<int> text_b)
{
  const int na = text_a.size();
  const int nb = text_b.size();

  vector<vector<int>> positions_a(alphabet);
  vector<vector<int>> positions_b(alphabet);

  for (int i=0; i<na; ++i) {
    positions_a[text_a[i]].push_back(i);
  }
  for (int i=0; i<nb; ++i) {
    positions_b[text_b[i]].push_back(i);
  }

  vector<int> next_a(alphabet, 0);
  vector<int> next_b(alphabet, 0);
  int pos_a = 0;
  int pos_b = 0;
  vector<int> result;

  while (pos_a < na && pos_b < nb) {
    int ta = text_a[pos_a];
    int tb = text_b[pos_b];
    if (ta == tb) {
      result.push_back(ta);
      ++pos_a;
      ++pos_b;
      continue;
    }

    for (int t : {ta, tb}) {
      while (next_a[t] < int(positions_a[t].size()) && positions_a[t][next_a[t]] < pos_a) {
        ++next_a[t];
      }
      while (next_b[t] < int(positions_b[t].size()) && positions_b[t][next_b[t]] < pos_b) {
        ++next_b[t];
      }
    }

    int left_ta_self = int(positions_a[ta].size()) - next_a[ta];
    int left_ta_opp = int(positions_b[ta].size()) - next_b[ta];
    int left_tb_self = int(positions_b[tb].size()) - next_b[tb];
    int left_tb_opp = int(positions_a[tb].size()) - next_a[tb];

    if (left_ta_opp == 0) {
      ++pos_a;
      continue;
    }
    if (left_tb_opp == 0) {
      ++pos_b;
      continue;
    }
    if (left_ta_self <= left_ta_opp) {
      if (left_tb_self <= left_tb_opp) {
        return {-1};
      } else {
        ++pos_b;
        continue;
      }
    } else {
      if (left_tb_self <= left_tb_opp) {
        ++pos_a;
        continue;
      } else {
        int pos_ta_opp = positions_b[ta][next_b[ta]];
        int pos_tb_opp = positions_a[tb][next_a[tb]];
        int last_ta = positions_a[ta][next_a[ta] + left_ta_self - left_ta_opp];
        int last_tb = positions_b[tb][next_b[tb] + left_tb_self - left_tb_opp];
        if (last_ta < pos_tb_opp) {
          if (last_tb < pos_ta_opp) {
            return {-1};
          } else {
            ++pos_b;
            continue;
          }
        } else {
          if (last_tb < pos_ta_opp) {
            ++pos_a;
            continue;
          } else {
            return {-1};
          }
        }
      }
    }
  }

  return result;
}


    vector<int> ucs(vector<int> a, vector<int> b) {
    vector<int> r1 = _ucs(a, b);
    std::reverse(a.begin(), a.end());
    std::reverse(b.begin(), b.end());
    vector<int> r2 = _ucs(a, b);
    std::reverse(r2.begin(), r2.end());
    return (r1 == r2) ? r1 : vector<int>({-1});
    }
}

namespace yiping_solexist {
typedef pair<int,int> pii;

struct Data
{
	int d;
	vector<int> a, cnt, rnk;
	vector< vector<int> > pos;
	
	Data(vector<int> _a, int _d)
	{
		a = _a; d = _d;
		cnt.resize(d); pos.resize(d); rnk.resize(a.size());
		for(int i=0; i<(int)a.size(); ++i)
		{
			int x = a[i];
			rnk[i] = cnt[x]; ++cnt[x];
			pos[x].emplace_back(i);
		}
	}
	bool check(int x,int cx,int y,int cy) const
	{
		if(cx > (int)pos[x].size() || cy > (int)pos[y].size())
			return 0;
		if(cx == 0 || cy == 0) return 1;
		return pos[x][cx - 1] < pos[y][(int)pos[y].size() - cy];
	}
};

vector<int> get_cand(const Data &aa, const Data &bb)
{
	int d = aa.d;
	vector<int> type(d), need(d);
	for(int i=0; i<d; ++i)
	{
		type[i] = aa.cnt[i] <= bb.cnt[i]? 0: 1;
		need[i] = min(aa.cnt[i], bb.cnt[i]);
	}
	
	vector<pii> veca, vecb;
	for(int i=0; i<(int)aa.a.size(); ++i)
		if(type[aa.a[i]] == 0)
			veca.emplace_back(aa.a[i], aa.rnk[i]);
	for(int i=0; i<(int)bb.a.size(); ++i)
		if(type[bb.a[i]] == 1)
			vecb.emplace_back(bb.a[i], bb.rnk[i]);
	
	auto check = [&] (pii x,pii y)
	{
		return aa.check(x.first, x.second + 1, y.first, need[y.first] - y.second)
		    && bb.check(x.first, x.second + 1, y.first, need[y.first] - y.second);
	};
	
	vector<int> c;
	int i = 0, j = 0;
	while(i<(int)veca.size() && j<(int)vecb.size())
	{
		bool tx = check(veca[i], vecb[j]);
		bool ty = check(vecb[j], veca[i]);
		
		if(tx == ty) return {-1};
		
		if(tx) c.emplace_back(veca[i].first), ++i;
		else c.emplace_back(vecb[j].first), ++j;
	}
	
	while(i<(int)veca.size()) c.emplace_back(veca[i].first), ++i;
	while(j<(int)vecb.size()) c.emplace_back(vecb[j].first), ++j;
	
	return c;
}

bool is_invalid(const vector<int> &c)
{
	return c.size() == 1 && c[0] == -1;
}

vector<int> ucs(vector<int> a, vector<int> b)
{
	int d = 0;
	for(auto t: a)
		d = max(d, t + 1);
	for(auto t: b)
		d = max(d, t + 1);
	
	Data aa(a, d), bb(b, d);
	
	auto c = get_cand(aa, bb);
	if(is_invalid(c)) return c;
	
	return c;
}
}

namespace yiping_wa {
    #include<bits/stdc++.h>
#include"hieroglyphs.h"
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;

struct Data
{
	int d;
	vector<int> a, cnt, rnk;
	vector< vector<int> > pos;
	
	Data(vector<int> _a, int _d)
	{
		a = _a; d = _d;
		cnt.resize(d); pos.resize(d); rnk.resize(a.size());
		for(int i=0; i<(int)a.size(); ++i)
		{
			int x = a[i];
			rnk[i] = cnt[x]; ++cnt[x];
			pos[x].emplace_back(i);
		}
	}
	bool check(int x,int cx,int y,int cy) const
	{
		if(cx > (int)pos[x].size() || cy > (int)pos[y].size())
			return 0;
		if(cx == 0 || cy == 0) return 1;
		return pos[x][cx - 1] < pos[y][(int)pos[y].size() - cy];
	}
	int get_next(int i,int x) const
	{
		auto it = lower_bound(pos[x].begin(), pos[x].end(), i);
		return it == pos[x].end()? (int)a.size(): *it;
	}
};

vector<int> get_cand(const Data &aa, const Data &bb)
{
	int d = aa.d;
	vector<int> type(d), need(d);
	for(int i=0; i<d; ++i)
	{
		type[i] = aa.cnt[i] <= bb.cnt[i]? 0: 1;
		need[i] = min(aa.cnt[i], bb.cnt[i]);
	}
	
	vector<pii> veca, vecb;
	for(int i=0; i<(int)aa.a.size(); ++i)
		if(type[aa.a[i]] == 0)
			veca.emplace_back(aa.a[i], aa.rnk[i]);
	for(int i=0; i<(int)bb.a.size(); ++i)
		if(type[bb.a[i]] == 1)
			vecb.emplace_back(bb.a[i], bb.rnk[i]);
	
	auto check = [&] (pii x,pii y)
	{
		return aa.check(x.first, x.second + 1, y.first, need[y.first] - y.second)
		    && bb.check(x.first, x.second + 1, y.first, need[y.first] - y.second);
	};
	
	vector<int> c;
	int i = 0, j = 0;
	while(i<(int)veca.size() && j<(int)vecb.size())
	{
		bool tx = check(veca[i], vecb[j]);
		bool ty = check(vecb[j], veca[i]);
		
		if(tx == ty) return {-1};
		
		if(tx) c.emplace_back(veca[i].first), ++i;
		else c.emplace_back(vecb[j].first), ++j;
	}
	
	while(i<(int)veca.size()) c.emplace_back(veca[i].first), ++i;
	while(j<(int)vecb.size()) c.emplace_back(vecb[j].first), ++j;
	
	return c;
}

bool is_invalid(const vector<int> &c)
{
	return c.size() == 1 && c[0] == -1;
}

bool is_subseq(const vector<int> &a, const vector<int> &c)
{
	int j = 0;
	for(int i=0; i<(int)a.size() && j<(int)c.size(); ++i)
		if(a[i] == c[j]) ++j;
	return j >= (int)c.size();
}

vector<int> ucs(vector<int> a, vector<int> b)
{
	int d = 0;
	for(auto t: a)
		d = max(d, t + 1);
	for(auto t: b)
		d = max(d, t + 1);
	
	Data aa(a, d), bb(b, d);
	
	auto c = get_cand(aa, bb);
	if(is_invalid(c)) return c;
	
	if(!is_subseq(a, c)) return {-1};
	if(!is_subseq(b, c)) return {-1};
	
	Data cc(c, d);
	
	const int LIM = 3000;
	const int pa = min(LIM, (int)a.size());
	const int pb = min(LIM, (int)b.size());
	
	int len_a = 0, len_b = 0;
	for(int i=(int)a.size()-1; i>=pa; --i)
		if(len_a < (int)c.size() && a[i] == c[(int)c.size() - len_a - 1])
			++len_a;
	for(int i=(int)b.size()-1; i>=pb; --i)
		if(len_b < (int)c.size() && b[i] == c[(int)c.size() - len_b - 1])
			++len_b;
	
	vector< vector<int> > dp(pa+1, vector<int>(pb+1));
	
	for(int i=0; i<=pa; ++i)
		for(int j=0; j<=pb; ++j)
		{
			if(i) dp[i][j] = max(dp[i][j], dp[i-1][j]);
			if(j) dp[i][j] = max(dp[i][j], dp[i][j-1]);
			if(i && j && a[i-1] == b[j-1])
			{
				dp[i][j] = max(dp[i][j], cc.get_next(dp[i-1][j-1], a[i-1]) + 1);
				if(dp[i][j] + min(len_a, len_b) > (int)c.size())
					return {-1};
			}
		}
	return c;
}
}

namespace felix_wa_jumps {
    

int ALPHABET_SIZE = 0;

bool is_subsequence(const vi& a, const vi& b) {
    int j = 0;
    for (int x : a) {
        if (j < (int)b.size() && b[j] == x) {
            j++;
        }
    }
    return j == (int)b.size();
}

vi get_candidate(const vi& a, const vi& b) {
    int n = a.size();
    int m = b.size();

    vi occ_a(ALPHABET_SIZE, 0);
    vi occ_b(ALPHABET_SIZE, 0);
    for (int i=0; i < n; ++i) {
        occ_a[a[i]]++;
    }
    for (int i=0; i < m; ++i) {
        occ_b[b[i]]++;
    }

    vi c;
    queue<int> qa;
    queue<int> qb;

    for (int i=0; i < n; ++i) {
        if (occ_a[a[i]] <= occ_b[a[i]]) {
            qa.push(i);
        }
    }
    for (int i=0; i < m; ++i) {
        if (occ_a[b[i]] > occ_b[b[i]]) {
            qb.push(i);
        }
    }

    int i_a_curr = 0;
    int i_b_curr = 0;
    int i_a_next = 0;
    int i_b_next = 0;
    vi occ_a_curr = vi(occ_a);
    vi occ_a_next = vi(occ_a);
    vi occ_b_curr = vi(occ_b);
    vi occ_b_next = vi(occ_b);

    while(!qa.empty() && !qb.empty()) {
        while(i_a_next < qa.front()) {
            occ_a_next[a[i_a_next]]--;
            i_a_next++;
        }
        while(i_b_next < qb.front()) {
            occ_b_next[b[i_b_next]]--;
            i_b_next++;
        }

        int x = a[i_a_next];
        int y = b[i_b_next];

        int occ_x = occ_a_next[x];
        int occ_y = occ_b_next[y];

        bool a_good = (occ_a_next[y] >= occ_y && occ_b_curr[x] > occ_b_next[x]);
        bool b_good = (occ_b_next[x] >= occ_x && occ_a_curr[y] > occ_a_next[y]);

        if (a_good && b_good) return {-1};
        if (!a_good && !b_good) return {-1};

        if(a_good) {
            c.push_back(x);
            qa.pop();
            while(i_a_curr <= i_a_next) {
                occ_a_curr[a[i_a_curr]]--;
                i_a_curr++;
            }
            while(b[i_b_curr] != x) {
                occ_b_curr[b[i_b_curr]]--;
                i_b_curr++;
            }
            occ_b_curr[b[i_b_curr]]--;
            i_b_curr++;
        }
        else {
            c.push_back(y);
            qb.pop();
            while(i_b_curr <= i_b_next) {
                occ_b_curr[b[i_b_curr]]--;
                i_b_curr++;
            }
            while(a[i_a_curr] != y) {
                occ_a_curr[a[i_a_curr]]--;
                i_a_curr++;
            }
            occ_a_curr[a[i_a_curr]]--;
            i_a_curr++;
        }
    }

    while(!qa.empty()) {
        c.push_back(a[qa.front()]);
        qa.pop();
    }
    while(!qb.empty()) {
        c.push_back(b[qb.front()]);
        qb.pop();
    }

    return ((is_subsequence(a, c) && is_subsequence(b, c)) ? c : vi({-1}));
}



bool verify_jump(const vi& a, const vi& b, const vi& c) {
    if (c == vi({-1})) return false;
    if (c == vi()) return true; 
    int n = a.size();
    int m = b.size();
    int l = c.size();

    vi occ_a(ALPHABET_SIZE, 0);
    vi occ_b(ALPHABET_SIZE, 0);
    vi occ_c(ALPHABET_SIZE, 0);
    vvi pos_a(ALPHABET_SIZE);
    vvi pos_b(ALPHABET_SIZE);
    vvi pos_c(ALPHABET_SIZE);
    for (int i=0; i < n; ++i) {
        occ_a[a[i]]++;
        pos_a[a[i]].push_back(i);
    }
    for (int i=0; i < m; ++i) {
        occ_b[b[i]]++;
        pos_b[b[i]].push_back(i);
    }
    for (int i=0; i < l; ++i) {
        occ_c[c[i]]++;
        pos_c[c[i]].push_back(i);
    }

    vi pos_c_idx(ALPHABET_SIZE);
    vi jump_left(l);
    for (int i=0; i < l; ++i) jump_left[i] = i;
    int c_idx = 0;

    for (int j=0; j < m; ++j) {
        int idx = pos_c_idx[b[j]];
        if (idx < occ_c[b[j]]) {
            jump_left[pos_c[b[j]][idx]] = min(jump_left[pos_c[b[j]][idx]], c_idx);
            pos_c_idx[b[j]]++;
        }
        if (b[j] == c[c_idx]) {
           // pos_c_idx[b[j]]++;
            c_idx++;
            if (c_idx == l) break;
        }
        
    }

    vi jump_right(l);
    for (int i=0; i < l; ++i) jump_right[i] = i;
    c_idx--;
    
    for (int i=n-1; i > -1; --i) {
         int idx = pos_c_idx[a[i]]-1;
        if (idx > -1) {
            jump_right[pos_c[a[i]][idx]] = max(jump_right[pos_c[a[i]][idx]], c_idx);
            pos_c_idx[a[i]]--;
        }
        if (a[i] == c[c_idx]) {
            //pos_c_idx[a[i]]--;
            c_idx--;
            if (c_idx < 0) break;
        }
        
    }

    vector<ii> stack_jump;


    for (int k=0; k < l; ++k) {
        while (!stack_jump.empty() && stack_jump.back().second < k) {
            stack_jump.pop_back();
        }
        if (!stack_jump.empty() && stack_jump.back().first >= jump_left[k]) {
            return false;
        }
        while (!stack_jump.empty() && stack_jump.back().second < jump_right[k]) {
            stack_jump.pop_back();
        }
        stack_jump.emplace_back(k, jump_right[k]);
        
    }

    
   

   
    return true;
}

bool verify(const vi& a, const vi& b, const vi& c) {
    return verify_jump(a, b, c) && verify_jump(b, a, c);
}

vector<int> ucs(vector<int> a, vector<int> b) {
    for (int x : a) ALPHABET_SIZE = max(ALPHABET_SIZE, x+1);
    for (int x : b) ALPHABET_SIZE = max(ALPHABET_SIZE, x+1);

    vi c = get_candidate(a, b);
    if (verify(a, b, c)) return c;
    return {-1};
}

}

namespace radewoosh {

    //Mateusz Radecki, O(n+m)
#include "hieroglyphs.h"

#include <bits/stdc++.h>
using namespace std;

using ll=long long;
using pii=pair<int,int>;
using pll=pair<ll,ll>;
using vi=vector<int>;
using vll=vector<ll>;
const int nax=1007*1000;

int n, m;

int ilea[nax];
int ileb[nax];

int potrzebuje[nax];

vi pozy[nax];

int potrzebujewa[nax];
int potrzebujewb[nax];

vi licz_harde(vi a, vi b)
{
	n=a.size();
	m=b.size();
	for (int i=0; i<nax; i++)
		ilea[i]=ileb[i]=0;
	for (int i : a)
		ilea[i]++;
	for (int i : b)
		ileb[i]++;
	vector<pii> za, zb;
	for (int i=0; i<n; i++)
		if (ilea[a[i]]<=ileb[a[i]])
			za.push_back({a[i], i});
	for (int i=0; i<m; i++)
		if (ilea[b[i]]>ileb[b[i]])
			zb.push_back({b[i], i});
	
	{
		potrzebujewa[zb.size()]=n;
		int wsk=zb.size();
		for (int i=n-1; i>=0; i--)
		{
			if (wsk && zb[wsk-1].first==a[i])
			{
				wsk--;
				potrzebujewa[wsk]=i;
			}
		}
	}
	{
		potrzebujewb[za.size()]=m;
		int wsk=za.size();
		for (int i=m-1; i>=0; i--)
		{
			if (wsk && za[wsk-1].first==b[i])
			{
				wsk--;
				potrzebujewb[wsk]=i;
			}
		}
	}
	
	vi ret;
	int wska=0;
	int wskb=0;
	int gdz=0;
	for (int i : a)
	{
		if (wska<(int)za.size() && i==za[wska].first)
		{
			while(gdz<m && b[gdz]!=i)
				gdz++;
			if (gdz==m)
				return {-1};
			gdz++;
			ret.push_back(i);
			wska++;
		}
		if (wskb<(int)zb.size() && i==zb[wskb].first && gdz<=zb[wskb].second && potrzebujewb[wska]>zb[wskb].second)
		{
			ret.push_back(i);
			gdz=zb[wskb].second+1;
			wskb++;
		}
	}
	
	if ((int)ret.size()<(int)za.size()+(int)zb.size())
		return {-1};
	
	return ret;
}

vi licz_proste(vi a, vi b, int numwy)
{
	n=a.size();
	m=b.size();
	for (int i=0; i<nax; i++)
		ilea[i]=ileb[i]=0;
	for (int i : a)
		ilea[i]++;
	for (int i : b)
		ileb[i]++;
	vector<pii> za;
	for (int i=0; i<n; i++)
		if (ilea[a[i]]<=ileb[a[i]]-numwy)
			za.push_back({a[i], i});
	
	{
		potrzebuje[za.size()]=m;
		int wsk=za.size();
		for (int i=m-1; i>=0; i--)
		{
			if (wsk && za[wsk-1].first==b[i])
			{
				wsk--;
				potrzebuje[wsk]=i;
			}
		}
		if (wsk)
			return {-1};
	}
	
	for (int i=0; i<nax; i++)
		pozy[i].clear();
	for (int i=m-1; i>=0; i--)
		pozy[b[i]].push_back(i);
	
	vi ret;
	int wsk=0;
	int gdz=0;
	for (int i : a)
	{
		if (wsk<(int)za.size() && i==za[wsk].first)
		{
			while(gdz<m && b[gdz]!=i)
				gdz++;
			assert(gdz<m);
			gdz++;
			ret.push_back(i);
			wsk++;
		}
		else
		{
			while(!pozy[i].empty() && pozy[i].back()<gdz)
				pozy[i].pop_back();
			if (!pozy[i].empty() && potrzebuje[wsk]>pozy[i].back())
			{
				ret.push_back(i);
				gdz=pozy[i].back()+1;
			}
		}
	}
	
	return ret;
}

vi prosty(vi a, vi b)
{
	n=a.size();
	m=b.size();
	for (int i=0; i<nax; i++)
		pozy[i].clear();
	for (int i=m-1; i>=0; i--)
		pozy[b[i]].push_back(i);
	int gdz=0;
	vi ret;
	for (int i : a)
	{
		while(!pozy[i].empty() && pozy[i].back()<gdz)
			pozy[i].pop_back();
		if (!pozy[i].empty())
		{
			ret.push_back(i);
			gdz=pozy[i].back()+1;
		}
	}
	return ret;
}

int zawiera(vi kto, vi kogo)
{
	int wsk=0;
	for (int i : kto)
		if (wsk<(int)kogo.size() && i==kogo[wsk])
			wsk++;
	return wsk==(int)kogo.size();
}

vi _ucs(vi a, vi b)
{
	vi odp=licz_harde(a, b);
	vi raz=licz_proste(a, b, 0);
	vi dwa=licz_proste(b, a, 1);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	vi trz=licz_proste(a, b, 0);
	vi czt=licz_proste(b, a, 1);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	reverse(trz.begin(), trz.end());
	reverse(czt.begin(), czt.end());
	
	vi x=prosty(a, b);
	vi y=prosty(b, a);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	vi z=prosty(a, b);
	vi w=prosty(b, a);
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	reverse(z.begin(), z.end());
	reverse(w.begin(), w.end());
	
	if (zawiera(odp, raz) && zawiera(odp, dwa) && zawiera(odp, trz) && zawiera(odp, czt) && zawiera(odp, x) && zawiera(odp, y) && zawiera(odp, z) && zawiera(odp, w))
		return odp;
	return {-1};
}

vi ucs(vi a, vi b) {
	//return _ucs(a, b);

    vi cpref;
    vi csuf;
    
    while (!a.empty() && !b.empty() && a.back() == b.back()) {
        csuf.push_back(a.back());
        a.pop_back();
        b.pop_back();
    }

    reverse(csuf.begin(), csuf.end());

    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());

    while (!a.empty() && !b.empty() && a.back() == b.back()) {
        cpref.push_back(a.back());
        a.pop_back();
        b.pop_back();
    }


    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());

    vi rc = _ucs(a, b);
    if (rc == vi({-1})) return rc;

    vi c;
    for (int x : cpref) c.push_back(x);
    for (int x : rc) c.push_back(x);
    for (int x : csuf) c.push_back(x);
    return c;
}

}

//erases non-common elements
void clean(vi& a, vi& b) {
    vi ap;
    vi bp;
    set<int> as;
    set<int> bs;
    for (int x : a) as.insert(x);
    for (int x : b) bs.insert(x);
    for (int x : a) if (bs.count(x)) ap.push_back(x);
    for (int x : b) if (as.count(x)) bp.push_back(x);
    swap(a, ap);
    swap(b, bp);
}

map<int, int> coordinate_compress(vi& a, vi& b) {
    int cc = 0;
    map<int, int> mp;
    map<int, int> rmp;
    for (int& x : a) {
        if (!mp.count(x)) {
            mp[x] = cc++;
            rmp[mp[x]] = x;
        }
        x = mp[x];
    }
    for (int& x : b) {
        if (!mp.count(x)) {
            mp[x] = cc++;
            rmp[mp[x]] = x;
        }
        x = mp[x];
    }
    return rmp;
}

bool compressed(const vi& a, const vi& b) {
    set<int> as;
    set<int> bs;
    int n = a.size();
    int m = b.size();
    for (int x : a) as.insert(x);
    for (int x : b) bs.insert(x);
    for (int x : a) {
        if (x >= n) return false;
        if (!bs.count(x)) return false;
    }
    for (int x : b) {
        if (x >= m) return false;
        if (!as.count(x)) return false;
    }
    return true;
}

bool is_subsequence(const vi& a, const vi& b) {
    int j = 0;
    for (int x : a) {
        if (j < (int)b.size() && b[j] == x) {
            j++;
        }
    }
    return j == (int)b.size();
}

bool _comp = false;


pair<bool, vi> solve(vi a, vi b) {
    if (!_comp) {
        if (a.empty() || b.empty()) {
            return pair<bool, vi>(true, {});
        }
        if (a.back() == b.back()) {
            vi v;
            while (!a.empty() && !b.empty() && a.back() == b.back()) {
                v.push_back(a.back());
                a.pop_back();
                b.pop_back();
            }
            reverse(v.begin(), v.end());
            auto p = solve(a, b);
            if (p.first) {
                for (int x : v) p.second.push_back(x);
            }
            return p;
        }
        if (a[0] == b[0]) {
            int idx = 0;
            while (idx < (int)min(a.size(), b.size()) && a[idx] == b[idx]) {
                idx++;
            }
            vi ap, bp;
            for (int i=idx; i < (int) a.size(); ++i) ap.push_back(a[i]);
            for (int i=idx; i < (int) b.size(); ++i) bp.push_back(b[i]);
            auto p = solve(ap, bp);
            if (p.first) {
                vi v;
                for (int i=0; i < idx; ++i) v.push_back(a[i]);
                for (int x : p.second) v.push_back(x);
                p.second = v;
            }
            return p;
        }
        if (!compressed(a, b)) {
            clean(a, b);
            if (a.empty() || b.empty()) {
                return pair<bool, vi>(true, {});
            }
            map<int, int> mp = coordinate_compress(a, b);
            _comp = true;
            auto p = solve(a, b);
            if (p.first)
                for (int& x : p.second) x = mp[x];
            return p;
        }
    }

    //End recursive solving part

    vector<function<vi(vi, vi)>> candidates_f = {tomek::ucs, yiping_solexist::ucs, yiping_wa::ucs, felix_wa_jumps::ucs, radewoosh::ucs};

    vi c = candidates_f[0](a, b);
    
    //cerr << "Candidate test" << endl;

    for (auto f : candidates_f) {
        if (c != f(a, b)) return pair<bool, vi>(false, {});
    }

    if (c == vi({-1})) return pair<bool, vi>(false, c);

    return pair<bool, vi>(true, c);
}

vector<int> ucs(vector<int> a, vector<int> b) {
    auto p = solve(a, b);
    if (p.first) {
        return p.second;
    }
    return {-1};
}

