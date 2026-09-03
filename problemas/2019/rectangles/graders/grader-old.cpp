#include "rect.h"
#include <cstdio>
#include <cassert>
// BEGIN SECRET
#include <string>
// END SECRET

using namespace std;

int main() {
	// BEGIN SECRET
	const string input_secret = "8d9a74d5-4c4b-4437-9c49-114beaeb8f1a";
	const string output_secret = "907404fa-efbb-4a2c-83b8-4c377409c80c";
	char secret[1000];
	assert(1 == scanf("%s", secret));
	if (string(secret) != input_secret) {
		printf("%s\n", output_secret.c_str());
		printf("SV\n");
		fclose(stdout);
		return 0;
	}
	// END SECRET
	int n, m;
	assert(2 == scanf("%d %d", &n, &m));
	vector<vector<int> > a(n, vector<int>(m));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			assert(1 == scanf("%d", &a[i][j]));
		}
	}
	fclose(stdin);

	long long result = count_rectangles(a);

	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	printf("OK\n");
	// END SECRET
	printf("%lld\n", result);
	fclose(stdout);
	return 0;
}
