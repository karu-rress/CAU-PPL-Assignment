/**
 * 
 * lexer.h
 * 
 * This file contains the declaration of the lexer class.
 * 
 */

#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>

#include "enums.h"

class lexer {
public:
    lexer() = default;
    explicit lexer(const std::string &in)
        : inputs(in) {
        get_char();
    }

    inline Token get_next_token() const {
        return next_token;
    }

    inline std::string get_token_string() const {
        return token_string;
    }

    void lexical();

private:
    char next_char = '\0'; // Next character
    CharClass char_class; // The class of the next character

    std::string token_string; // Next token string
    Token next_token; // The type of the next token

    std::string inputs; // Input string

    inline void add_char() {
        token_string += next_char;
    }

    void get_char();

    inline void get_non_blank() {
        while (next_char <= 32 && next_char != '\0') {
            get_char();
        }
    }

    void lookup(char ch);
};

#endif