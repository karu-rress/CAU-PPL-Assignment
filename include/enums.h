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
    LETTER,
    DIGIT,
    UNKNOWN,
    END,
};

// Token types
enum class Token : int {
    CONST,
    IDENT,
    ASSIGN_OP,
    ASSIGN_OP_INCOMPLETE,
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
std::ostream &operator<<(std::ostream &os, CharClass char_class);
std::ostream &operator<<(std::ostream &os, Token token);

#endif // __ENUMS_H__