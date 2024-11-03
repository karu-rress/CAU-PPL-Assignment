/**
 * 
 * parser.h
 * 
 * This file contains the declaration of the parser class.
 * 
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <map>
#include <optional>
#include <sstream>
#include <string>

#include "lexer.h"

class parser {
public:
    using data_t = double;

    explicit parser(const std::string &filename);

    void parse();

private:
    std::map<std::string, std::optional<data_t>> symbol_table;
    lexer lex;
    std::ostringstream current_statement;
    std::string error_message;
    int ids = 0, consts = 0, ops = 0;

    // <program> → <statements>
    inline void program() {
        statements();
    }

    // <statements> → <statement> | <statement><semi_colon><statements>
    void statements();

    // <statement> → <ident><assignment_op><expression>
    void statement();

    // <expression> → <term><term_tail>
    std::optional<data_t> expression();

    // <term_tail> → <add_op><term><term_tail> | ε
    std::optional<data_t> term_tail();

    // <term> → <factor><factor_tail>
    std::optional<data_t> term();

    // <factor_tail> → <mult_op><factor><factor_tail> | ε
    std::optional<data_t> factor_tail();

    // <factor> → <left_paren><expression><right_paren> | <ident> | <const>
    std::optional<data_t> factor();

    std::string ident(bool add = false);

    void take_next_token();
};

#endif // __PARSER_H__