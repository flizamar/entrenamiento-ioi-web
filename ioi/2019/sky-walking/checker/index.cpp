#include "testlib.h"
using namespace std;

static string output_secret = "d99f6469-b674-4a96-9c39-7bc072e22a4f";

int main(int argc, char * argv[])
{
	registerChecker("walk", argc, argv);

	readBothSecrets(output_secret);
	readBothGraderResults();

	compareRemainingLines(3);
}
