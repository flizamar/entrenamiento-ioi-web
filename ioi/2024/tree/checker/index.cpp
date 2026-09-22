#include "testlib.h"
#include <string>

const std::string output_secret = "11XNDQnkdGXK8y3iaqfMvWKu4vqrBbz1";

int main(int argc, char* argv[]) {
    registerChecker("tree", argc, argv);

    readBothSecrets(output_secret);
    readBothGraderResults();
    compareRemainingLines(3);
}
