#include "hieroglyphs.h"
#include <vector>
#include <algorithm>
using std::vector;

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