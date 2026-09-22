#include "sphinx.h"

using namespace std;

vector<int> find_colours(int /*N*/, vector<int> /*X*/, vector<int> /*Y*/) {
  int f0 = perform_experiment({-1, 0}) - 1;
  int f1 = perform_experiment({0, -1}) - 1;
  return {f0, f1};
}
