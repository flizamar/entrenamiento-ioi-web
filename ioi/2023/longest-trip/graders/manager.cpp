#include "testlib.h"

#include <cassert>
#include <csignal>
#include <cstdio>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <deque>

inline FILE *openFile(const char *name, const char *mode)
{
    FILE *file = fopen(name, mode);
    if (!file)
    {
        quitf(_fail, "Could not open file '%s' with mode '%s'", name, mode);
    }
    return file;
}

static inline constexpr int maxNumberOfCalls = 32640;
static inline constexpr int maxTotalNumberOfCalls = 150000;
static inline constexpr int maxTotalNumberOfLandmarksInCalls = 1500000;
static int call_counter = 0;
static int total_call_counter = 0;
static int landmark_counter = 0;

int calculateLMax(int N, const std::vector<std::vector<int>> &U)
{
    std::deque<int> line1;
    line1.clear();
    std::deque<int> line2;
    line2.clear();

    line1.push_back(0);

    for (int i = 1; i < N; i++)
    {
        if (U[i][line1.back()] == 1)
        {
            line1.push_back(i);
        }
        else if (!line2.empty() && U[i][line2.back()])
        {
            line2.push_back(i);
        }
        else
        {
            while (!line2.empty())
            {
                line1.push_back(line2.back());
                line2.pop_back();
            }
            line2.push_back(i);
        }
    }

    if (line1.size() < line2.size())
        swap(line1, line2);

    for (int i : line1)
    {
        for (int j : line2)
        {
            if (U[std::max(i, j)][std::min(i, j)] == 1)
                return N;
        }
    }
    return line1.size();
}

int main(int argc, char *argv[])
{
    testlibMode = _checker;
    ouf.mode = _output;

    if (argc < 3)
    {
        quit(_fail, "Insufficient number of args for manager of 'longesttrip'");
    }

    { // Keep alive on broken pipes
        struct sigaction sa;
        sa.sa_handler = SIG_IGN;
        sigaction(SIGPIPE, &sa, NULL);
    }

    FILE *fout = openFile(argv[2], "a");
    FILE *fin = openFile(argv[1], "r");

    char subtask[10];
    int C;
    assert(2 == scanf("%s %d", subtask, &C));

    fprintf(fout, "%d\n", C);
    fflush(fout);

    int maximumCalls = 0;
    for (int c = 0; c < C; ++c)
    {
        int N, D;
        assert(2 == scanf("%d %d", &N, &D));

        std::vector<bool> present(N, false);
        std::vector<std::vector<int>> U(N);
        for (int i = 1; i < N; ++i)
        {
            U[i].resize(i);
            for (int j = 0; j < i; ++j)
            {
                assert(1 == scanf("%d", &U[i][j]));
            }
        }

        int lMax = calculateLMax(N, U);

        fprintf(fout, "%d %d\n", N, D);
        fflush(fout);

        while (true)
        {
            {
                std::string input_secret = "3kC2Ia2048BfyJVGojMUKKtilctlZKcB";
                char secret[1000];
                if (fscanf(fin, "%s", secret) != 1)
                {
                    quit(_pv, "Could not read secret (possibly, an unexpected termination"
                              " of the program).");
                }
                if (std::string(secret) != input_secret)
                {
                    quit(_pv, "Secret mismatch (possible tampering with the output).");
                }
            }

            int op;
            if (fscanf(fin, "%d", &op) != 1)
            {
                quit(_fail, "Could not read op.");
            }

            if (op == 0)
            {
                ++call_counter, ++total_call_counter;
                if (call_counter > maxNumberOfCalls || total_call_counter > maxTotalNumberOfCalls)
                {
                    quit(_pv, "too many calls");
                }

                int nA, nB;
                if (fscanf(fin, "%d %d", &nA, &nB) != 2)
                {
                    quit(_fail, "Could not read arrays.");
                }

                landmark_counter += nA + nB;
                if (landmark_counter > maxTotalNumberOfLandmarksInCalls)
                {
                    quit(_pv, "too many elements");
                }

                if (nA <= 0 || nB <= 0 || nA + nB > N)
                {
                    quit(_pv, "invalid array");
                }

                std::vector<int> A(nA);
                for (int i = 0; i < nA; ++i)
                {
                    if (fscanf(fin, "%d", &A[i]) != 1)
                    {
                        quit(_fail, "Could not read array A.");
                    }
                    if (A[i] < 0 || N <= A[i])
                    {
                        quit(_pv, "invalid array");
                    }
                    if (present[A[i]])
                    {
                        quit(_pv, "invalid array");
                    }
                    present[A[i]] = true;
                }
                for (int i = 0; i < nA; ++i)
                {
                    present[A[i]] = false;
                }
                std::vector<int> B(nB);
                for (int i = 0; i < nB; ++i)
                {
                    if (fscanf(fin, "%d", &B[i]) != 1)
                    {
                        quit(_fail, "Could not read array B.");
                    }
                    if (B[i] < 0 || N <= B[i])
                    {
                        quit(_pv, "invalid array");
                    }
                    if (present[B[i]])
                    {
                        quit(_pv, "invalid array");
                    }
                    present[B[i]] = true;
                }
                for (int i = 0; i < nB; ++i)
                {
                    present[B[i]] = false;
                }

                for (int i = 0; i < nA; ++i)
                {
                    for (int j = 0; j < nB; ++j)
                    {
                        if (A[i] == B[j])
                        {
                            quit(_pv, "non-disjoint arrays");
                        }
                    }
                }

                bool connected = false;
                for (int i = 0; i < nA && !connected; ++i)
                {
                    for (int j = 0; j < nB && !connected; ++j)
                    {
                        if (U[std::max(A[i], B[j])][std::min(A[i], B[j])] == 1)
                        {
                            connected = true;
                        }
                    }
                }

                fprintf(fout, "%d\n", connected);
                fflush(fout);
            }
            else if (op == 1)
            {
                int l;
                if (fscanf(fin, "%d", &l) != 1)
                {
                    quit(_fail, "Could not read answer.");
                }

                if (l < 0 || N < l)
                {
                    quit(_wa);
                }

                std::vector<int> answer(l);
                for (int i = 0; i < l; ++i)
                {
                    if (fscanf(fin, "%d", &answer[i]) != 1)
                    {
                        quit(_fail, "Could not read answer.");
                    }
                    if (answer[i] < 0 || answer[i] >= N)
                    {
                        quit(_wa);
                    }
                    for (int j = 0; j < i; ++j)
                    {
                        if (answer[i] == answer[j])
                        {
                            quit(_wa);
                        }
                    }
                    if (i > 0 && U[std::max(answer[i], answer[i - 1])][std::min(answer[i], answer[i - 1])] == 0)
                    {
                        quit(_wa);
                    }
                }

                if (std::string(subtask) == "D1Half")
                {
                    if (l < (lMax + 1) / 2)
                        quit(_wa);
                }
                else
                {
                    if (l < lMax)
                        quit(_wa);
                }

                maximumCalls = std::max(maximumCalls, call_counter);
                call_counter = 0;

                int continueWithNextCall = 1;
                fprintf(fout, "%d\n", continueWithNextCall);
                fflush(fout);

                break;
            }
            else
            {
                quit(_fail, "Invalid op.");
            }
        }
    }

    if (std::string(subtask) == "D1")
    {
        if (maximumCalls <= 400)
        {
            quit(_ok);
        }
        else if (maximumCalls <= 550)
        {
            quitp(0.75);
        }
        else if (maximumCalls <= 2750)
        {
            quitp(0.5);
        }
        else
        {
            quitp(1.0 / 3.0);
        }
    }
    else
    {
        quit(_ok);
    }

    return 0;
}
