#include "testlib.h"
using namespace std;

static string output_secret = "907404fa-efbb-4a2c-83b8-4c377409c80c";

int main(int argc, char * argv[])
{
	registerChecker("rect", argc, argv);

	readBothSecrets(output_secret);
	readBothGraderResults();

	compareRemainingLines(3);
}
