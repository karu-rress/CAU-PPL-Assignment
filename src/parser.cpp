/**
 *
 * parser.cpp
 *
 * This file contains the implementation of the parser class.
 *
 */

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>

#include "../include/parser.h"

using namespace std;
using data_t = double;

////////////// PUBLIC //////////////

parser::parser(const string &filename) {
    if (ifstream fin(filename); fin.is_open()) {
        stringstream ss;
        ss << fin.rdbuf();
        fin.close();

        lex = lexer(ss.str());
    }
    else {
        cerr << "An error has occured while opening file." << endl;
        exit(1);
        return;
    }
}

void parser::parse() {
    take_next_token();

    program();

    // Program ends

    cout << "Result ==> ";
    ranges::for_each(symbol_table, [](auto &&symbol) {
        if (cout << symbol.first << ": "; symbol.second)
            cout << *symbol.second << "; ";
        else
            cout << "Unknown; ";
    });
    cout << endl;
}

////////////// PRIVATE //////////////

// <statements> → <statement> | <statement><semi_colon><statements>
void parser::statements() {
    if (lex.get_next_token() == Token::END)
        return;

    statement();

    // Statement ends
    cout << current_statement.str() << endl;
    cout << format("ID: {}; CONST: {}; OP: {};", ids, consts, ops) << endl;
    cout << (error_message.empty() ? "(OK)" : error_message) << endl;

    ids = consts = ops = 0;
    error_message.clear();
    current_statement.str("");
    current_statement.clear();

    if (lex.get_next_token() == Token::SEMICOLON) {
        take_next_token(); // semi_colon()

        statements();
    }
}

// <statement> → <ident><assignment_op><expression>
void parser::statement() {
    string &&id = ident(true); // Adding identifier to symbol table

    if (lex.get_next_token() == Token::ASSIGN_OP_INCOMPLETE) {
        error_message = "(Warning) Use of '=' instead of ':='.";
    }
    take_next_token(); // assign_operator()

    symbol_table[id] = expression();
}

// <expression> → <term><term_tail>
optional<data_t> parser::expression() {
    optional<data_t> term1 = term();

    Token t = lex.get_next_token(); // operator

    optional<data_t> term2 = term_tail();
    
    if (t == Token::UNKNOWN) {
        error_message = "(Error) Unknown operator.";
        return nullopt;
    }

    if (!term1)
        return nullopt;

    if (!term2)
        return term1;

    return (t == Token::ADD_OP) ? *term1 + *term2 : *term1 - *term2;
}

// <term_tail> → <add_op><term><term_tail> | ε
optional<data_t> parser::term_tail() {
    if (auto token = lex.get_next_token(); token == Token::ADD_OP || token == Token::SUB_OP) {
        ops++;
        take_next_token(); // operator

        optional<data_t> term1 = term();
        Token t = lex.get_next_token(); // operator
        optional<data_t> term2 = term_tail();

        if (!term1)
            return nullopt;

        if (!term2)
            return term1;

        return (t == Token::ADD_OP) ? *term1 + *term2 : *term1 - *term2;
    }
    else if (token == Token::MULT_OP || token == Token::DIV_OP) {
        error_message = "(Warning) Ignoring extra operator '" + lex.get_token_string() + "'.";

        ops++;

        take_next_token();
        return term_tail();
    }
    else if (token == Token::IDENT || token == Token::CONST) {
        error_message = "(Warning) Missing operator. Ignoring extra term.";

        if (token == Token::IDENT)
            ids++;
        else
            consts++;

        take_next_token();
        return term_tail();
    }
    else if (token == Token::ASSIGN_OP) {
        error_message = "(Error) Missing ';' between two statements.";

        take_next_token();
        return expression();
    }
    else if (token == Token::UNKNOWN) {
        take_next_token();
        term_tail();
        return nullopt;
    }
    else
        return nullopt;
}

// <term> → <factor><factor_tail>
optional<data_t> parser::term() {
    optional<data_t> factor1 = factor();

    Token t = lex.get_next_token(); // operator

    optional<data_t> factor2 = factor_tail();

    if (!factor1)
        return nullopt;

    if (!factor2)
        return factor1;

    return (t == Token::MULT_OP) ? *factor1 * *factor2 : *factor1 / *factor2;
}

// <factor_tail> → <mult_op><factor><factor_tail> | ε
optional<data_t> parser::factor_tail() {
    if (lex.get_next_token() == Token::MULT_OP || lex.get_next_token() == Token::DIV_OP) {
        ops++;
        take_next_token();

        optional<data_t> factor1 = factor();
        Token t = lex.get_next_token(); // operator
        optional<data_t> factor2 = factor_tail();

        if (!factor1)
            return nullopt;

        if (!factor2)
            return factor1;

        if (t == Token::DIV_OP && *factor2 == 0) {
            error_message = "(Warning) Division by zero. Ignoring operation.";
            return factor1;
        }

        return (t == Token::MULT_OP) ? *factor1 * *factor2 : *factor1 / *factor2;
    }
    else
        return nullopt;
}

// <factor> → <left_paren><expression><right_paren> | <ident> | <const>
optional<data_t> parser::factor() {
    if (auto token = lex.get_next_token(); token == Token::LEFT_PAREN) {
        take_next_token(); // left_paren()

        if (auto ret = expression(); lex.get_next_token() == Token::RIGHT_PAREN) {
            take_next_token(); // right_paren()
            return ret;
        }
        else {
            error_message = "(Warning) Missing ')'. Assuming ')' at the end.";
            return ret;
        }
    }
    else if (token == Token::IDENT) {
        return symbol_table[ident()];
    }
    else if (token == Token::CONST) {
        consts++;
        data_t result = stod(lex.get_token_string());

        take_next_token();
        return result;
    }
    else {
        if (token == Token::ADD_OP || token == Token::SUB_OP
            || token == Token::MULT_OP || token == Token::DIV_OP) {
            error_message = format("(Warning) Ignoring extra operator '{}'.", lex.get_token_string());

            ops++;

            take_next_token();
            return factor();
        }
        else {
            error_message = "(Error) expected a '(', an identifier, or a constant.";
            return nullopt;
        }
    }
}

string parser::ident(bool add) {
    ids++;
    string &&id = lex.get_token_string();

    auto symbol_exists = [=, this](auto symbol) {
        return symbol.first == id;
    };

    if (symbol_table.count(id) == 0) {
        if (add) {
            symbol_table[id] = 0.0;
        }
        else {
            error_message = format("(Error) Undefined identifier: '{}'.", id);
            symbol_table[id] = nullopt;
        }
    }

    take_next_token();
    return id;
}

void parser::take_next_token() {
    lex.lexical();

    if (string &&token = lex.get_token_string(); token == ";") {
        current_statement << "\b" << token;
    }
    else if (token == "(") {
        current_statement << token;
    }
    else if (token == ")") {
        current_statement << "\b" << token << " ";
    }
    else if (token != "EOF") {
        current_statement << lex.get_token_string() << " ";
    }
}