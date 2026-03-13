# pragma once

#include <string>
#include <vector>
#include "token.hpp"

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

        vector<Token> tokenize() {
            vector<Token> tokens = vector<Token>();
            tokens.reserve(INITIAL_CAPACITY);

            char buffer[256];
            string currentToken = "";
            while (fgets(buffer, sizeof(buffer), sourceFile) != NULL) {

                for (size_t i = 0; buffer[i] != '\0'; i++) {
                    char currentChar = buffer[i];
                    TokenType currentCharType = Token::getTypeFromChar(currentChar);

                    if (currentCharType == TokenType::UNKNOWN) {
                        currentToken += currentChar;
                    } else {
                        if (!currentToken.empty()) {
                            tokens.push_back(Token(currentToken));
                            currentToken = "";
                        }
                        tokens.push_back(Token(string(1, currentChar)));
                    }
                }
            }

            return tokens;
        }
};