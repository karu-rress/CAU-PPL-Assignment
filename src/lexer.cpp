/**
 * 
 * lexer.cpp
 * 
 * This file contains the implementation of the lexer class.
 * 
 */

#include <utility>

#include "../include/lexer.h"

////////////// PUBLIC //////////////

void lexer::lexical() {
    token_string.clear();
    token_string.shrink_to_fit();

    switch (get_non_blank(); char_class) {
    case CharClass::LETTER:
        add_char();
        get_char();
        while (char_class == CharClass::LETTER || char_class == CharClass::DIGIT) {
            add_char();
            get_char();
        }
        next_token = Token::IDENT;
        break;

    case CharClass::DIGIT:
        add_char();
        get_char();
        while (char_class == CharClass::DIGIT) {
            add_char();
            get_char();
        }
        next_token = Token::CONST;
        break;

    case CharClass::UNKNOWN:
        lookup(next_char);
        get_char();
        break;

    case CharClass::END:
        next_token = Token::END;
        token_string = "EOF";
        break;

    [[unlikely]] default:
        std::unreachable();
        break;
    }
}

////////////// PRIVATE //////////////

void lexer::lookup(char ch) {
    switch (ch) {
    case '+':
        add_char();
        next_token = Token::ADD_OP;
        break;

    case '-':
        add_char();
        next_token = Token::SUB_OP;
        break;

    case '*':
        add_char();
        next_token = Token::MULT_OP;
        break;

    case '/':
        add_char();
        next_token = Token::DIV_OP;
        break;

    case '(':
        add_char();
        next_token = Token::LEFT_PAREN;
        break;

    case ')':
        add_char();
        next_token = Token::RIGHT_PAREN;
        break;

    case ':':
        add_char();
        get_char();

        if (next_char == '=') {
            add_char();
            next_token = Token::ASSIGN_OP;
        }
        else {
            next_token = Token::UNKNOWN;
        }
        break;

    case ';':
        add_char();
        next_token = Token::SEMICOLON;
        break;

    default:
        add_char();
        next_token = Token::END;
        break;
    }
}

void lexer::get_char() {
    if (inputs.empty()) {
        next_char = '\0';
        char_class = CharClass::END;
    }
    else {
        next_char = inputs.front();
        inputs.erase(0, 1);
        if (isalpha(next_char) || next_char == '_') { // C identifier rules
            char_class = CharClass::LETTER;
        }
        else if (isdigit(next_char)) {
            char_class = CharClass::DIGIT;
        }
        else {
            char_class = CharClass::UNKNOWN;
        }
    }
}