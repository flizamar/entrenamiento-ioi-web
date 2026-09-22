#include "testlib.h"

#include <string>
const std::string output_secret = "p89vHUOQJ7iyHtdrgGXzKx8iRtXLL6wH";

int main(int argc, char *argv[])
{
    registerChecker("beechtree", argc, argv);
    readBothSecrets(output_secret);
    readBothGraderResults();
    compareRemainingLines(2);
}
