#ifndef __ENUMS_H__
#define __ENUMS_H__

#include <iostream>

// Character classes
enum class CharClass : int {
    LETTER = 1,
    DIGIT = 2,
    UNKNOWN = 4,
    END = 8,
};

// Token types
enum class Token : int {
    CONST,
    IDENT,
    ASSIGN_OP,
    SEMICOLON,
    ADD_OP,
    SUB_OP,
    MULT_OP,
    DIV_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    UNKNOWN,
    END,
};

// Overloaded operators for enums
std::ostream &operator<<(std::ostream &os, const CharClass &char_class);
std::ostream &operator<<(std::ostream &os, const Token &token);

#endif // __ENUMS_H__