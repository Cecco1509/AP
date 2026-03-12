# pragma once

#include <string>
#include <vector>

using namespace std;

const size_t INITIAL_CAPACITY = 255;

class Lexer {
    private:
        FILE* sourceFile;
    public:

        Lexer(const string& source){
            sourceFile = fopen(source.c_str(), "r");
            if (sourceFile == NULL) {
                perror("Error opening source file");
                exit(EXIT_FAILURE);
            }
        }

        vector<string> tokenize(){
            vector<string> tokens = vector<string>();
            tokens.reserve(INITIAL_CAPACITY);

            
            // Read the source file
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), sourceFile) != NULL) {
                // Tokenize the line
                char* token = strtok(buffer, " \t\n");
                while (token != NULL) {
                    tokens.push_back(string(token));
                    token = strtok(NULL, " \t\n");
                }
            }

            return tokens;
        }
};