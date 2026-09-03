#include "rect.h"
#include <cstdio>
#include <iostream>
#include <cassert>
// BEGIN SECRET
#include <string>
// END SECRET

using namespace std;

// BEGIN SECRET
static char *ptr;

inline void readinp() {
	fseek(stdin, 0, SEEK_END);
	int inSize = ftell(stdin);
	rewind(stdin);
	ptr = (char*)malloc(inSize);
	fread(ptr, 0x1, inSize, stdin);
}

inline void freadn(register int *Q) {
	for(; *ptr < '0'; ++ptr);
	for(*Q = *ptr++ - '0'; *ptr >= '0'; *Q = *Q * 10 + *ptr++ - '0');
}

inline void freadstr(register char *s) {
	for(; *ptr <= ' '; ++ptr);
	for(*s++ = *ptr++; *ptr > ' '; *s++ = *ptr++);
	*s++ = 0;
}

// END SECRET
int main() {
	// BEGIN SECRET
	readinp();
	const string input_secret = "8d9a74d5-4c4b-4437-9c49-114beaeb8f1a";
	const string output_secret = "907404fa-efbb-4a2c-83b8-4c377409c80c";
	char secret[1000];
	freadstr(secret);
	std::cerr << secret << endl;
	std::cerr << input_secret << endl;
	if (string(secret) != input_secret) {
		printf("%s\n", output_secret.c_str());
		printf("SV\n");
		fclose(stdout);
		return 0;
	}
	// END SECRET
	int n, m;
	freadn(&n);
	freadn(&m);
	vector<vector<int> > a(n, vector<int>(m));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			freadn(&a[i][j]);
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
