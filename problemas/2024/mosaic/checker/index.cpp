#include "testlib.h"
#include <string>

const std::string output_secret = "Wm5rkGNobnYjFI7TIY17RAm6FAQ2LlO9";

int main(int argc, char *argv[]) {
	registerChecker("mosaic", argc, argv);
	readBothSecrets(output_secret);
	readBothGraderResults();
	compareRemainingLines(3);
}
