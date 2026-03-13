#pragma once

#include <stdlib.h>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

enum class TokenType {
    SPACE, // " "
    TAB,   // "\t"
    NEWLINE, // "\n"
    ATX, // #
    WORD, // Just a word without any special characters
    BACKTICK, // `
    TILDE, // ~
    DASH, // -
    UNDERSCORE, // _
    STAR, // *
    PLUS, // +
    NUMBER, // [0-9]+
    POINT, // .
    PIPE, // |
    COLON, // :
    SQUAREBRACKETOPEN, // [
    SQUAREBRACKETCLOSE, // ]
    PARENOPEN, // (
    PARENCLOSE, // )
    BACKSLASH, // '\'
    ANGLEBRACKETOPEN, // <
    ANGLEBRACKETCLOSE, // >
    AT, // @
    AND, // &
    SEMICOLON, // ;
    x, // x
    X, // X
    UNKNOWN // Error token, should never be returned by the lexer
};

class Token {
private:
    TokenType type;
    string value;
public:
    Token(TokenType type, const string& value) : type(type), value(value) {}
    Token(const string& value) : type(getTypeFromString(value)), value(value) {}

    TokenType getType() const {
        return type;
    }

    string getValue() const {
        return value;
    }

    // Prints a token in a format T{type, value}
    string toString() const {
        return "T{" + to_string(static_cast<int>(type)) + ", " + value + "}";
    }

    static TokenType getTypeFromString(const string& str) {
        
        TokenType t = TokenType::UNKNOWN;
        if (str.length() == 1) {
            t = getTypeFromChar(str[0]);
        }

        if (t  == TokenType::UNKNOWN) {

            if (str.find_first_not_of("0123456789") == string::npos) {
                return TokenType::NUMBER;
            }

            return TokenType::WORD;
        }

        return t;
    }

    static TokenType getTypeFromChar(const char& c) {
        
        switch (c)
        {
        case ' ':
            return TokenType::SPACE;
        case '\t':
            return TokenType::TAB;
        case '\n':
            return TokenType::NEWLINE;
        case '#':
            return TokenType::ATX;
        case '`':
            return TokenType::BACKTICK;
        case '~':
            return TokenType::TILDE;
        case '-':
            return TokenType::DASH;
        case '_':
            return TokenType::UNDERSCORE;
        case '*':
            return TokenType::STAR;
        case '+':
            return TokenType::PLUS;
        case '.':
            return TokenType::POINT;
        case '|':
            return TokenType::PIPE;
        case ':':
            return TokenType::COLON;
        case '[':
            return TokenType::SQUAREBRACKETOPEN;
        case ']':
            return TokenType::SQUAREBRACKETCLOSE;
        case '(':
            return TokenType::PARENOPEN;
        case ')':
            return TokenType::PARENCLOSE;
        case '\\':
            return TokenType::BACKSLASH;
        case '<':
            return TokenType::ANGLEBRACKETOPEN;
        case '>':
            return TokenType::ANGLEBRACKETCLOSE;
        case '@':
            return TokenType::AT;
        case '&':
            return TokenType::AND;
        case ';':
            return TokenType::SEMICOLON;
        case 'x':       
            return TokenType::x;
        case 'X':
            return TokenType::X;
        default:
            return TokenType::UNKNOWN;
        }
    }
};