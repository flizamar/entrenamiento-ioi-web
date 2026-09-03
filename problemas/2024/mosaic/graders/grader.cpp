#include "mosaic.h"
#include <cassert>
#include <cstdio>
#include <vector>
// BEGIN SECRET
#include <cstdlib>
#include <string>

namespace {

void write_grader_result(std::string grader_result, std::string grader_msg = "") {
  const std::string output_secret = "Wm5rkGNobnYjFI7TIY17RAm6FAQ2LlO9";
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

  const std::string input_secret = "njJ9Z7VxxKGR6SUcJMgdzy3qMz4JZ1Tq";
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
  std::vector<int> X(N), Y(N);
  for (int i = 0; i < N; i++)
    assert(1 == scanf("%d", &X[i]));
  for (int i = 0; i < N; i++)
    assert(1 == scanf("%d", &Y[i]));
  int Q;
  assert(1 == scanf("%d", &Q));
  std::vector<int> T(Q), B(Q), L(Q), R(Q);
  for (int k = 0; k < Q; k++)
    assert(4 == scanf("%d%d%d%d", &T[k], &B[k], &L[k], &R[k]));
  fclose(stdin);
  // BEGIN SECRET
  exit_allowed = false;
  // END SECRET

  std::vector<long long> C = mosaic(X, Y, T, B, L, R);

  // BEGIN SECRET
  if ((int)C.size() != Q) {
    write_grader_result("WA", "Returned array C is not of length Q");
    fclose(stdout);
    authorized_exit(0);
  }
  write_grader_result("OK");
  // END SECRET
  int S = (int)C.size();
  for (int k = 0; k < S; k++)
    printf("%lld\n", C[k]);
  fclose(stdout);

  // BEGIN SECRET
  exit_allowed = true;
  // END SECRET
  return 0;
}
