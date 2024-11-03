/**
 *
 * enums.cpp
 *
 * This file contains the overloaded operators
 * for the enums used in the lexer and parser.
 *
 */

#include <array>
#include <string_view>
#include <utility>

#include "../include/enums.h"

std::ostream &operator<<(std::ostream &os, CharClass char_class) {
    static constexpr std::array<std::string_view, 4> char_class_names = {
        "LETTER",
        "DIGIT",
        "UNKNOWN",
        "END",
    };

    return os << char_class_names[static_cast<int>(char_class)];
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    static constexpr std::array<std::string_view, 12> token_names = {
        "CONST",
        "IDENT",
        "ASSIGN_OP",
        "SEMICOLON",
        "ADD_OP",
        "SUB_OP",
        "MULT_OP",
        "DIV_OP",
        "LEFT_PAREN",
        "RIGHT_PAREN",
        "UNKNOWN",
        "END",
    };

    return os << token_names[static_cast<int>(token)];
}