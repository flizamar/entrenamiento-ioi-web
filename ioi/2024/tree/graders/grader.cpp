#include "tree.h"
#include <cassert>
#include <cstdio>
// BEGIN SECRET
#include <cstdlib>
#include <string>

namespace {

void write_grader_result(std::string grader_result, std::string grader_msg = "") {
  const std::string output_secret = "11XNDQnkdGXK8y3iaqfMvWKu4vqrBbz1";
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

  const std::string input_secret = "ZYKrr4gCMcKeyfk6kbZU5k4ZyW3sAGT0";
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
  std::vector<int> P(N);
  P[0] = -1;
  for (int i = 1; i < N; i++)
    assert(1 == scanf("%d", &P[i]));
  std::vector<int> W(N);
  for (int i = 0; i < N; i++)
    assert(1 == scanf("%d", &W[i]));
  int Q;
  assert(1 == scanf("%d", &Q));
  std::vector<int> L(Q), R(Q);
  for (int j = 0; j < Q; j++)
    assert(2 == scanf("%d%d", &L[j], &R[j]));
  fclose(stdin);
  // BEGIN SECRET
  exit_allowed = false;
  // END SECRET

  init(P, W);
  std::vector<long long> A(Q);
  for (int j = 0; j < Q; j++)
    A[j] = query(L[j], R[j]);

  // BEGIN SECRET
  write_grader_result("OK");
  // END SECRET
  for (int j = 0; j < Q; j++)
    printf("%lld\n", A[j]);
  fclose(stdout);

  // BEGIN SECRET
  exit_allowed = true;
  // END SECRET
  return 0;
}
