/**
 * 
 * enums.cpp
 * 
 * This file contains the overloaded operators
 * for the enums used in the lexer and parser.
 * 
 */

#include <utility>

#include "../include/enums.h"

std::ostream &operator<<(std::ostream &os, const CharClass &char_class) {
    switch (char_class) {
    case CharClass::LETTER:
        os << "LETTER";
        break;
    case CharClass::DIGIT:
        os << "DIGIT";
        break;
    case CharClass::UNKNOWN:
        os << "UNKNOWN";
        break;
    case CharClass::END:
        os << "END";
        break;
    [[unlikely]] default:
        std::unreachable();
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    switch (token) {
    case Token::CONST:
        os << "CONST";
        break;
    case Token::IDENT:
        os << "IDENT";
        break;
    case Token::ASSIGN_OP:
        os << "ASSIGN_OP";
        break;
    case Token::SEMICOLON:
        os << "SEMICOLON";
        break;
    case Token::ADD_OP:
        os << "ADD_OP";
        break;
    case Token::MULT_OP:
        os << "MULT_OP";
        break;
    case Token::LEFT_PAREN:
        os << "LEFT_PAREN";
        break;
    case Token::RIGHT_PAREN:
        os << "RIGHT_PAREN";
        break;
    case Token::UNKNOWN:
        os << "UNKNOWN";
        break;
    case Token::END:
        os << "END";
        break;
    [[unlikely]] default:
        std::unreachable();
    }
    return os;
}