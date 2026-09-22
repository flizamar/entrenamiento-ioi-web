#include "testlib.h"

#include <string>
const std::string output_secret = "mGlgT4yvr1qPbquFwkxRVh9hMn0Mrxoz";

int main(int argc, char *argv[])
{
    registerChecker("overtaking", argc, argv);
    readBothSecrets(output_secret);
    readBothGraderResults();
    compareRemainingLines(3);
}
