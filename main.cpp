#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/*
<program> → <statements>
<statements>→ <statement> | <statement><semi_colon><statements>
<statement> → <ident><assignment_op><expression>
<expression> → <term><term_tail>
<term_tail> → <add_op><term><term_tail> | ε
<term> → <factor> <factor_tail>
<factor_tail> → <mult_op><factor><factor_tail> | ε
<factor> → <left_paren><expression><right_paren> | <ident> | <const>
<const> → any decimal numbers
<ident> → any names conforming to C identifier rules
<assignment_op> → :=
<semi_colon> → ;
<add_operator> → + | -
<mult_operator> → * | /
<left_paren> → (
<right_paren> → )
*/

enum class Token {
    LETTER,
    DIGIT,
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
    END
};

class LLParser {
public:
    LLParser(ifstream &file)
        : file(file) { }

    void parse() {
        get_char();
        do {
            lex();
        } while (next_token != Token::END);
    }

    void program() {
        statements();
    }

    void statements() {
        statement();
        if (next_token == Token::SEMICOLON) {
            lex();
            statements();
        }
    }

    void statement() {
        if (next_token == Token::IDENT) {
            lex();
            if (next_token == Token::ASSIGN_OP) {
                lex();
                expression();
            }
        }
    }

    void expression() {
        term();
        term_tail();
    }

    void term_tail() {
        if (next_token == Token::ADD_OP || next_token == Token::SUB_OP) {
            lex();
            term();
            term_tail();
        }
    }

    void term() {
        factor();
        factor_tail();
    }

    void factor_tail() {
        if (next_token == Token::MULT_OP || next_token == Token::DIV_OP) {
            lex();
            factor();
            factor_tail();
        }
    }

    void factor() {
        if (next_token == Token::LEFT_PAREN) {
            lex();
            expression();
            if (next_token == Token::RIGHT_PAREN) {
                lex();
            }
        }
        else if (next_token == Token::IDENT || next_token == Token::CONST) {
            lex();
        }
    }

    void lex() {
        get_non_blank();
        switch (charClass) {
        case Token::LETTER:
            add_char();
            get_char();
            while (charClass == Token::LETTER || charClass == Token::DIGIT) {
                add_char();
                get_char();
            }
            next_token = Token::IDENT;
            break;

        case Token::DIGIT:
            add_char();
            get_char();
            while (charClass == Token::DIGIT) {
                add_char();
                get_char();
            }
            next_token = Token::CONST;
            break;

        case Token::UNKNOWN:
            lookup(nextChar);
            get_char();
            break;

        case Token::END:
            next_token = Token::END;
            token_string = "EOF";
            break;
        }
        cout << "Next token is: " << (int)next_token << ", Next lexeme is " << token_string << endl;
    }

    Token lookup(char ch) {
        switch (ch) {
        case '(':
            add_char();
            next_token = Token::LEFT_PAREN;
            break;

        case ')':
            add_char();
            next_token = Token::RIGHT_PAREN;
            break;

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

        case ':':
            add_char();
            get_char();
            if (nextChar == '=') {
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
        return next_token;
    }

private:
    ifstream &file;
    string token_string; // lexeme
    char nextChar;
    Token charClass, next_token;

    inline void add_char() {
        token_string += nextChar;
    }

    inline void get_non_blank() {
        while (isspace(nextChar)) {
            file.get(nextChar);
        }
    }

    void get_char() {
        if (file.get(nextChar)) {
            if (isalpha(nextChar)) {
                charClass = Token::LETTER;
            }
            else if (isdigit(nextChar)) {
                charClass = Token::DIGIT;
            }
            else {
                charClass = Token::UNKNOWN;
            }
        }
        else {
            charClass = Token::END;
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cout << "Error: Unable to open file" << endl;
        return 1;
    }

    LLParser parser(file);
    parser.parse();

    return 0;
}