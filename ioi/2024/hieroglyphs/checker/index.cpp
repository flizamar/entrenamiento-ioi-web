#include "testlib.h"
#include <string>

const std::string output_secret = "IyRwUZ9rsuq5tjuK54lpSvORqGQyBWEZ";

int main(int argc, char *argv[]) {
	registerChecker("hieroglyphs", argc, argv);
	readBothSecrets(output_secret);
	readBothGraderResults();
	compareRemainingLines(3);
}
