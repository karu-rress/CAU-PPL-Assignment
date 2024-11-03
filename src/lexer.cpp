/**
 *
 * lexer.cpp
 *
 * This file contains the implementation of the lexer class.
 *
 */

#include <map>
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
        break;
    }
}

////////////// PRIVATE //////////////

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

void lexer::lookup(char ch) {
    static const std::map<char, Token> lookup_table = {
        { '+', Token::ADD_OP },
        { '-', Token::SUB_OP },
        { '*', Token::MULT_OP },
        { '/', Token::DIV_OP },
        { '(', Token::LEFT_PAREN },
        { ')', Token::RIGHT_PAREN },
        { ';', Token::SEMICOLON },
    };

    if (lookup_table.contains(ch)) {
        add_char();
        next_token = lookup_table.at(ch);
    }
    else if (ch == ':') {
        add_char();
        get_char();

        if (next_char == '=') {
            add_char();
            next_token = Token::ASSIGN_OP;
        }
        else
            next_token = Token::UNKNOWN;
    }
    else {
        add_char();
        next_token = Token::END;
    }
}