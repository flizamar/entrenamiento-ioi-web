#include "hieroglyphs.h"
#include <cassert>
#include <cstdio>
#include <vector>
// BEGIN SECRET
#include <cstdlib>
#include <string>

namespace {

void write_grader_result(std::string grader_result, std::string grader_msg = "") {
  const std::string output_secret = "IyRwUZ9rsuq5tjuK54lpSvORqGQyBWEZ";
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

  const std::string input_secret = "vHwzrZUx9chlYIJ7zODvOcQbZwj3OxhB";
  char secret[1000];
  assert(1 == scanf("%999s", secret));
  if (std::string(secret) != input_secret) {
    write_grader_result("PV", "Possible tampering with the input");
    fclose(stdout);
    authorized_exit(0);
  }
  // END SECRET
  int N, M;
  assert(2 == scanf("%d%d", &N, &M));
  std::vector<int> A(N), B(M);
  for (int i = 0; i < N; i++)
    assert(1 == scanf("%d", &A[i]));
  for (int j = 0; j < M; j++)
    assert(1 == scanf("%d", &B[j]));
  fclose(stdin);
  // BEGIN SECRET
  exit_allowed = false;
  // END SECRET

  std::vector<int> R = ucs(A, B);

  // BEGIN SECRET
  if ((int)R.size() > std::min(N, M)) {
    write_grader_result("WA", std::string("R.size() (") + std::to_string(R.size()) +
                                  std::string(") is larger than min(N, M)"));
    fclose(stdout);
    authorized_exit(0);
  }
  write_grader_result("OK");
  // END SECRET
  int T = (int)R.size();
  printf("%d\n", T);
  for (int i = 0; i < T; i++)
    printf("%s%d", (i == 0 ? "" : " "), R[i]);
  printf("\n");
  fclose(stdout);

  // BEGIN SECRET
  exit_allowed = true;
  // END SECRET
  return 0;
}
