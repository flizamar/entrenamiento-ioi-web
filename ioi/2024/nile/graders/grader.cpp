#include "nile.h"
#include <cassert>
#include <cstdio>
#include <vector>
// BEGIN SECRET
#include <cstdlib>
#include <string>

namespace {

void write_grader_result(std::string grader_result, std::string grader_msg = "") {
  const std::string output_secret = "4FBwMMwcdlgPodEtp0owUlxQgA5SuIlq";
  printf("%s\n", output_secret.c_str());
  printf("%s\n", grader_result.c_str());
  if (!grader_msg.empty())
    printf("%s\n", grader_msg.c_str());
  fflush(stdout);
}

bool exit_allowed = false;

void authorized_exit(int exit_code) {
  exit_allowed = true;
  exit(exit_code);
}

void check_exit_protocol() {
  if (!exit_allowed)
    write_grader_result("PV", "Solution called exit()");
}

} // namespace
// END SECRET

int main() {
  // BEGIN SECRET
  atexit(check_exit_protocol);
  at_quick_exit(check_exit_protocol);
  exit_allowed = true;

  const std::string input_secret = "1U22aPSRkiGgB7zXbfJs9IqSfacMQ9t7";
  char secret[1000];
  assert(1 == scanf("%999s", secret));
  if (std::string(secret) != input_secret) {
    write_grader_result("PV", "Possible tampering with the input");
    fclose(stdout);
    authorized_exit(0);
  }
  // END SECRET
  int N;
  assert(1 == scanf("%d", &N));
  std::vector<int> W(N), A(N), B(N);
  for (int i = 0; i < N; i++)
    assert(3 == scanf("%d%d%d", &W[i], &A[i], &B[i]));
  int Q;
  assert(1 == scanf("%d", &Q));
  std::vector<int> E(Q);
  for (int j = 0; j < Q; j++)
    assert(1 == scanf("%d", &E[j]));
  fclose(stdin);
  // BEGIN SECRET
  exit_allowed = false;
  // END SECRET

  std::vector<long long> R = calculate_costs(W, A, B, E);

  // BEGIN SECRET
  if ((int)R.size() != Q) {
    write_grader_result("WA", "Returned array R is not of length Q");
    fclose(stdout);
    authorized_exit(0);
  }
  write_grader_result("OK");
  // END SECRET
  int S = (int)R.size();
  for (int j = 0; j < S; j++)
    printf("%lld\n", R[j]);
  fclose(stdout);

  // BEGIN SECRET
  exit_allowed = true;
  // END SECRET
  return 0;
}
