#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

enum class Token {
    IDENT,
    CONST,
    ASSIGN_OP,
    SEMICOLON,
    ADD_OP,
    MULT_OP,
    LEFT_PAREN,
    RIGHT_PAREN,
    END,
    UNKNOWN
};

class Lexer {
public:
    Lexer(const string &in)
        : inputs(in) { }

    int lexical() {
        get_non_blank();
        switch (char_class) {
        case LETTER:
            add_char();
            get_char();
            while (char_class == LETTER || char_class == DIGIT) {
                add_char();
                get_char();
            }
            next_token = static_cast<int>(Token::IDENT);
            break;

        case DIGIT:
            add_char();
            get_char();
            while (char_class == DIGIT) {
                add_char();
                get_char();
            }
            next_token = static_cast<int>(Token::CONST);
            break;

        case UNKNOWN:
            lookup(next_char);
            get_char();
            break;

        case END:
            next_token = static_cast<int>(Token::END);
            token_string = "EOF";
            break;

        default:
            break;
        }

        cout << "Next token is: " << next_token << ", Next lexeme is " << token_string << endl;
        token_string.clear();
        return next_token;
    }

    void analyze() {
        get_char();
        do {
            lexical();
        } while (next_token != static_cast<int>(Token::END));
    }

    int getNextToken() { return lexical(); }

private:
    int next_token;
    int char_class;
    string token_string;
    char next_char;
    string inputs;

    enum CharClass {
        LETTER,
        DIGIT,
        UNKNOWN,
        END
    };

    void lookup(char ch) {
        switch (ch) {
        case '+':
        case '-':
            add_char();
            next_token = static_cast<int>(Token::ADD_OP);
            break;
        case '*':
        case '/':
            add_char();
            next_token = static_cast<int>(Token::MULT_OP);
            break;
        case '(':
            add_char();
            next_token = static_cast<int>(Token::LEFT_PAREN);
            break;
        case ')':
            add_char();
            next_token = static_cast<int>(Token::RIGHT_PAREN);
            break;
        case ':':
            add_char();
            get_char();
            if (next_char == '=') {
                add_char();
                next_token = static_cast<int>(Token::ASSIGN_OP);
            }
            else {
                next_token = static_cast<int>(Token::UNKNOWN);
            }
            break;
        case ';':
            add_char();
            next_token = static_cast<int>(Token::SEMICOLON);
            break;
        default:
            add_char();
            next_token = static_cast<int>(Token::END);
            break;
        }
    }

    void add_char() { token_string += next_char; }

    void get_char() {
        if (inputs.empty()) {
            char_class = END;
        }
        else {
            next_char = inputs.front();
            inputs.erase(0, 1);
            if (isalpha(next_char))
                char_class = LETTER;
            else if (isdigit(next_char))
                char_class = DIGIT;
            else
                char_class = UNKNOWN;
        }
    }

    void get_non_blank() {
        while (isspace(next_char))
            get_char();
    }
};

class LLParser {
public:
    LLParser(const string &input)
        : lexer(input) {
        next_token = lexer.getNextToken();
    }

    void parse() { program(); }

private:
    Lexer lexer;
    int next_token;

    void advance() { next_token = lexer.getNextToken(); }

    void program() { statements(); }

    void statements() {
        statement();
        if (next_token == static_cast<int>(Token::SEMICOLON)) {
            semi_colon();
            statements();
        }
    }

    void statement() {
        ident();
        assignment_op();
        expression();
    }

    void expression() {
        term();
        term_tail();
    }

    void term_tail() {
        if (next_token == static_cast<int>(Token::ADD_OP)) {
            add_operator();
            term();
            term_tail();
        }
    }

    void term() {
        factor();
        factor_tail();
    }

    void factor_tail() {
        if (next_token == static_cast<int>(Token::MULT_OP)) {
            mult_operator();
            factor();
            factor_tail();
        }
    }

    void factor() {
        if (next_token == static_cast<int>(Token::LEFT_PAREN)) {
            left_paren();
            expression();
            right_paren();
        }
        else if (next_token == static_cast<int>(Token::IDENT)) {
            ident();
        }
        else if (next_token == static_cast<int>(Token::CONST)) {
            constant();
        }
    }

    void constant() {
        if (next_token == static_cast<int>(Token::CONST)) {
            advance();
        }
        else {
            error("Expected constant");
        }
    }

    void ident() {
        if (next_token == static_cast<int>(Token::IDENT)) {
            advance();
        }
        else {
            error("Expected identifier");
        }
    }

    void assignment_op() {
        if (next_token == static_cast<int>(Token::ASSIGN_OP)) {
            advance();
        }
        else {
            error("Expected ':='");
        }
    }

    void semi_colon() {
        if (next_token == static_cast<int>(Token::SEMICOLON)) {
            advance();
        }
        else {
            error("Expected ';'");
        }
    }

    void add_operator() {
        if (next_token == static_cast<int>(Token::ADD_OP)) {
            advance();
        }
        else {
            error("Expected '+' or '-'");
        }
    }

    void mult_operator() {
        if (next_token == static_cast<int>(Token::MULT_OP)) {
            advance();
        }
        else {
            error("Expected '*' or '/'");
        }
    }

    void left_paren() {
        if (next_token == static_cast<int>(Token::LEFT_PAREN)) {
            advance();
        }
        else {
            error("Expected '('");
        }
    }

    void right_paren() {
        if (next_token == static_cast<int>(Token::RIGHT_PAREN)) {
            advance();
        }
        else {
            error("Expected ')'");
        }
    }

    void error(const string &msg) {
        cerr << "Syntax error: " << msg << endl;
        exit(1);
    }
};

int main() {
    string input = "x := 5 + (3 * 4);";
    LLParser parser(input);
    parser.parse();
    cout << "Parsing completed successfully." << endl;
    return 0;
}
