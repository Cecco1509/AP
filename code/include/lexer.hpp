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
                perror(string("Error opening source file: " + source).c_str());
                exit(EXIT_FAILURE);
            }
        }

        vector<Token> tokenize() {
            vector<Token> tokens = vector<Token>();
            tokens.reserve(INITIAL_CAPACITY);

            char buffer[256];
            string currentWord = "";
            while (fgets(buffer, sizeof(buffer), sourceFile) != NULL) {

                for (size_t i = 0; buffer[i] != '\0'; i++) {
                    char currentChar = buffer[i];
                    TokenType currentCharType = Token::getTypeFromChar(currentChar);

                    // Special handling for 'x' and 'X' to avoid treating them as WORD tokens
                    if (currentCharType == TokenType::x || currentCharType == TokenType::X) {
                        
                        if (tokens.size() > 0 && tokens.at(tokens.size() - 1).getType() == TokenType::SQUAREBRACKETCLOSE) {
                            tokens.push_back(Token(string(1, currentChar)));
                            continue;
                        }

                        currentWord += currentChar;
                        continue;
                    }


                    if (currentCharType == TokenType::UNKNOWN) {
                        currentWord += currentChar;
                    } else {
                        if (!currentWord.empty()) {
                            tokens.push_back(Token(currentWord));
                            currentWord = "";
                        }
                        tokens.push_back(Token(string(1, currentChar)));
                    }
                }
            }

            return tokens;
        }
};