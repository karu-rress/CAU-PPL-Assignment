#include <iostream>
#include <sstream>

#include "include/parser.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    parser parser(argv[1]);
    parser.parse();

    return 0;
}