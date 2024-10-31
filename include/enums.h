/**
 * 
 * enums.h
 * 
 * This file contains the declaration of the enums
 * used in the lexer and parser.
 * 
 */

#ifndef __ENUMS_H__
#define __ENUMS_H__

#include <ostream>

// Character classes
enum class CharClass : int {
    LETTER = 1,
    DIGIT = 2,
    UNKNOWN = 4,
    END = 8,
};

// Token types
enum class Token : int {
    CONST = 10,
    IDENT = 11,
    ASSIGN_OP = 21,
    SEMICOLON = 22,
    ADD_OP = 31,
    SUB_OP = 32,
    MULT_OP = 41,
    DIV_OP = 42,
    LEFT_PAREN = 51,
    RIGHT_PAREN = 52,
    UNKNOWN = 99,
    END = -1,
};

// Overloaded operators for enums
std::ostream &operator<<(std::ostream &os, const CharClass &char_class);
std::ostream &operator<<(std::ostream &os, const Token &token);

#endif // __ENUMS_H__