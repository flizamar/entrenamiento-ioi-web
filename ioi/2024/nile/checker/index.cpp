#include "testlib.h"

#include <string>
const std::string output_secret = "4FBwMMwcdlgPodEtp0owUlxQgA5SuIlq";

int main(int argc, char *argv[])
{
    registerChecker("nile", argc, argv);
    readBothSecrets(output_secret);
    readBothGraderResults();
    compareRemainingLines(3);
}