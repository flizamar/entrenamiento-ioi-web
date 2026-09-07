#include "overtaking.h"
#include <cassert>
#include <cstdio>
#include <vector>
// BEGIN SECRET
#include <string>
// END SECRET

int main()
{
    // BEGIN SECRET
    const std::string input_secret = "XwWPuInrOjpekAwGKojzwKw3yVDtdkGS";
    const std::string output_secret = "mGlgT4yvr1qPbquFwkxRVh9hMn0Mrxoz";
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
    int L, N, X, M, Q;
    assert(5 == scanf("%d %d %d %d %d", &L, &N, &X, &M, &Q));
    std::vector<long long> T(N);
    for (int i = 0; i < N; i++)
        assert(1 == scanf("%lld", &T[i]));
    std::vector<int> W(N);
    for (int i = 0; i < N; i++)
        assert(1 == scanf("%d", &W[i]));
    std::vector<int> S(M);
    for (int i = 0; i < M; i++)
        assert(1 == scanf("%d", &S[i]));
    std::vector<long long> Y(Q);
    for (int i = 0; i < Q; i++)
        assert(1 == scanf("%lld", &Y[i]));

    fclose(stdin);

    init(L, N, T, W, X, M, S);
    std::vector<long long> res(Q);
    for (int i = 0; i < Q; i++)
        res[i] = arrival_time(Y[i]);

    // BEGIN SECRET
    printf("%s\n", output_secret.c_str());
    printf("OK\n");
    // END SECRET
    for (int i = 0; i < Q; i++)
        printf("%lld\n", res[i]);
    fclose(stdout);
    return 0;
}
