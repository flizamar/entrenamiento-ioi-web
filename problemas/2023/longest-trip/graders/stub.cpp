#include "longesttrip.h"

#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

bool are_connected(std::vector<int> A, std::vector<int> B)
{
    {
        const std::string output_secret = "3kC2Ia2048BfyJVGojMUKKtilctlZKcB";
        printf("%s\n", output_secret.c_str());
    }
    int nA = A.size(), nB = B.size();
    printf("0 %d %d", nA, nB);
    for (int x : A)
    {
        printf(" %d", x);
    }
    for (int x : B)
    {
        printf(" %d", x);
    }
    printf("\n");
    fflush(stdout);

    int connected;
    if (scanf("%d", &connected) != 1)
    {
        exit(0);
    }
    return connected == 1;
}

int main(int argc, char *argv[])
{
    assert(argc >= 3);
    stdin = fopen(argv[1], "r");
    stdout = fopen(argv[2], "a");

    int C;
    assert(1 == scanf("%d", &C));

    for (int k = 0; k < C; ++k)
    {
        int N, D;
        assert(2 == scanf("%d %d", &N, &D));

        std::vector<int> answer = longest_trip(N, D);
        {
            const std::string output_secret = "3kC2Ia2048BfyJVGojMUKKtilctlZKcB";
            printf("%s\n", output_secret.c_str());
        }
        int l = answer.size();
        printf("1 %d", l);
        for (int x : answer)
        {
            printf(" %d", x);
        }
        printf("\n");
        fflush(stdout);
        int continueWithNextCall = 0;
        if (scanf("%d", &continueWithNextCall) != 1)
        {
            exit(0);
        }
    }

    return 0;
}
