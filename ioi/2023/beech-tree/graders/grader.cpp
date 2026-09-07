#include "beechtree.h"
#include <cassert>
#include <cstdio>
// BEGIN SECRET
#include <string>
// END SECRET

int main()
{
    // BEGIN SECRET
    const std::string input_secret = "j2DRV0nYbs0y1xUYUGaiqtOKUU9vM9zi";
    const std::string output_secret = "p89vHUOQJ7iyHtdrgGXzKx8iRtXLL6wH";
    char secret[1000];
    assert(1 == scanf("%999s", secret));
    if (std::string(secret) != input_secret)
    {
        printf("%s\n", output_secret.c_str());
        printf("PV\n");
        printf("Possible tampering with the input\n");
        fclose(stdout);
        return 0;
    }
    // END SECRET
    int N, M;
    assert(2 == scanf("%d %d", &N, &M));
    std::vector<int> P(N);
    for (int i = 0; i < N; ++i)
        assert(1 == scanf("%d", &P[i]));
    std::vector<int> C(N);
    for (int i = 0; i < N; ++i)
        assert(1 == scanf("%d", &C[i]));

    fclose(stdin);

    std::vector<int> res = beechtree(N, M, P, C);

    // BEGIN SECRET
    printf("%s\n", output_secret.c_str());
    printf("OK\n");
    // END SECRET
    int n = res.size();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
            printf(" ");
        printf("%d", res[i]);
    }
    printf("\n");
    fclose(stdout);

    return 0;
}
