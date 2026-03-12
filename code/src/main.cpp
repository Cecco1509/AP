#include <stdio.h>
#include "../include/lexer.hpp"

const string OUTPUT_PATH = "output.json";

using namespace std;

// Args
// ?[1] optional flag for JSON output or HTML
// [2] source file path
// [3] output file path
int main(int argc, char* argv[]) {

    string sourceFilePath = "";
    string outputFilePath = OUTPUT_PATH;
    string outputFormat = "json"; // default output format
    
    if (argc < 2 || argc >= 4) {
        printf("Usage: %s [-json|-html] <source_file_path> [output_file_path]\n", argv[0]);
        return 1;
    }

    if (argc > 2) {
        string flag = argv[1];
        if (flag == "-json") {
            outputFormat = "json";
        } else if (flag == "-html") {
            outputFormat = "html";
        } else {
            printf("Unknown flag: %s\n", flag.c_str());
            return 1;
        }
        sourceFilePath = argv[2];

        if (argc > 3) {
            outputFilePath = argv[3];
        }
    }else {
        sourceFilePath = argv[1];
    }

    // Call the Lexer
    Lexer lexer = Lexer(sourceFilePath);
    vector<string> tokens = lexer.tokenize();

    for (const string& token : tokens) {
        printf("%s\n", token.c_str());
    }

    // Create the parser with the output file path
    // Call the parser with the tokens and the output format

    // Write the result to the output file

    return 0;
}