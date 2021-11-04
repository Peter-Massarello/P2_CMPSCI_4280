#include "scanner.hpp"
#include "parser.hpp"

using namespace std;

int main(int argc, char** argv){
    vector<Token*> tokens;
    // Read from command line and determine path based on arg count
    if (argc == 1) { 
        tokens = createTokenVector();
        parse(tokens);
    } else if (argc == 2) { 
        tokens = createTokenVector(argv[1]);
        parse(tokens);
    } else cout << "ERROR: Too many args given, existing...\n";

    freeTokenVector(tokens);
    return 0;
}