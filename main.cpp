#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <optional>
#include <sstream>
#include <string>

#include "enums.h"

using namespace std;

/*
파일을 통해 입력된 프로그램이 아래에 제시된 LL(1) 문법을 따르는지 여부를 판단하는
파서(Parser)를 Recursive Descent Parsing 기법을 이용하여 작성하고, 파싱된 결과를
이용하여 입력된 프로그램의 결과를 출력하시오.

 ② 자신의 학번으로 명명된 폴더 내에 작성된 모든 소스 코드 저장
  예) 20234748_나선우

 ※ 주의!  프로그램을 평가할때, 컴파일이 되지 않으면 채점되지 않으니,
   꼭 외부문서(External  Document)에 제출하는 프로그램을 프로그램 수행에 필요한 요소와
   컴파일부터  수행시키는 방법까지 잘 설명 해주셔야 합니다.
 ※ 입력: 임의의 이름이 부여된 텍스트 파일 (Command Line 파라미터로 파일명이 주어짐.
  (예: "python main.py eval1.txt")

 ※ 출력: 주어진 문법에 따라 입력파일에 저장되어 있는 프로그램을 분석한다.
   파싱(parsing)되는 과정을 <처리 예> 와 같이 출력하고, 문법 오류 없이 파싱된 경우,
   파싱 트리를 구축하고, 이 트리를 이용하여 <ident>들의 최종값을 출력  한다.
   => 출력 형식:
   “프로그램에서 읽은 라인”
   “ID: {개수}; CONST: {개수}; OP: {개수};”
   “파싱 결과 (OK), (WARNING), (ERROR)”
   “Result ==> {변수1}: {최종값}; {변수2}: {최종값}; {변수3}: {결과값};”

 ※ 처리 조건
 각 문장들이 파싱된 이후, 입력된 문장과 그 문장에 포함된 식별자(IDENT),  숫자(CONST),
 그리고 연산자(OP)의 개수를 출력한다.  파싱된 문장이 문법에 적합하면 ‘<OK>’, 적합하지 않으면
  적절한 에러 ‘<ERROR>’  메시지나 경고 ‘<WARNING>’ 메시지를 출력한다.
  위에서 주어진 문법에 의거 오류가 발견된 경우, 오류를 가능한 한 복구한 다음, 파싱을
  계속한다. 예를 들어, x = a + + b일 경우, “+” 연산자가 한 개가 더 존재하므로 “+” 기호를
  제거한 다음 적절한 ‘경고(Warning) 메시지’를 출력한 후, 파싱을 계속한다. 오류 복구가
  불가능한 경우는 ‘에러(Error) 메시지’를 출력하고 파싱을 계속하되, 이 경우 해당 식별자(<IDENT>)의
   값은 ‘Unknown’으로 결정된다. 문장이 포함하고 있는 모든 오류에  대해 경고 또는 에러 메시지를
   출력해야 한다. 가능한 한 오류를 복구해야 하고, 불가능한  경우만 에러 메시지를 출력한다. 에러나
   오류 메시지 내용은 각자 적절히 정의한다(문서에  표기해야 함). 처리된 오류 각각에 대해 추가
   점수가 부여된다. 프로그램에 속한 일부 문장이  문법에 적합하지 않더라도 오류 복구를 통해
   프로그램이 끝까지 전부 파싱되어야 한다.  파싱 트리 생성 후, 모든 <IDENT> 값이 출력되어야
   한다. 단, <IDENT>의 값이 정의되지  않은 경우, “Unknown"으로 표시한다.  <IDENT>의 현재
   값을 저장하기 위해 심볼 테이블(symbol table)을 구축해야 한다.  입력 스트림에서 ASCII
   코드값이 32 이하인 것은 모두 white-space로 간주되며,  white-space는 각 token을 구별하는
   용도 이외에는 모두 무시된다.  어휘분석기(lexical analyzer)의 소스 코드는 정수 변수
   next_token, 문자열 변수  token_string, 함수 lexical()을 포함하여야  한다.  함수
   lexical()은 입력 스트림을  분석하여 하나의 lexeme을 찾아낸  뒤,  그것의  token type을
   next_token에 저장하고,  lexeme 문자열을 token_string에 저장하는 함수이다. 기타 구현 시
   요구되는 세부 사항은  직접 결정하고, Internal 및 External Document에 기술한다.
*/

////////////// LEXICAL ANALYSIS //////////////

class Lexer {
public:
    Lexer() = default;

    Lexer(const string &in)
        : inputs { in } {
        get_char();
    }

    inline Token get_next_token() const {
        return next_token;
    }

    inline string get_token_string() const {
        return token_string;
    }

    void lexical() {
        token_string.clear();

        get_non_blank();
        switch (char_class) {
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

        default:
            break;
        }
    }

private:
    Token next_token;
    CharClass char_class;
    string token_string;
    char next_char = '\0';
    string inputs;

    void lookup(char ch) {
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

    inline void add_char() {
        token_string += next_char;
    }

    void get_char() {
        if (inputs.empty()) {
            next_char = '\0';
            char_class = CharClass::END;
        }
        else {
            next_char = inputs.front();
            inputs.erase(0, 1);
            if (isalpha(next_char)) {
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

    inline void get_non_blank() {
        while (isspace(next_char)) {
            get_char();
        }
    }
};

class Parser {
public:
    Parser(const string &filename)
        : fin(filename) {
        if (!fin.is_open()) {
            cerr << "File open error" << endl;
            return;
        }

        // Load file content
        stringstream ss;
        ss << fin.rdbuf();
        input = ss.str();
    }

    void parse() {
        lexer = Lexer(input);
        get_next_token();

        program();
    }

    ~Parser() {
        fin.close();
    }

private:
    ifstream fin;
    list<pair<string, optional<int>>> symbol_table;
    Lexer lexer;
    string input;
    ostringstream current_statement;
    string error_message;
    int ids = 0, consts = 0, ops = 0;

    // <program> → <statements>
    void program() {
        statements();

        // END OF PROGRAM
        cout << "RESULT ==> ";
        for (auto &[var, value] : symbol_table) {
            cout << var << ": ";
            if (value) 
                cout << *value;
            else
                cout << "Unknown";
            cout << "; ";
        }
    }

    // <statements> → <statement> | <statement><semi_colon><statements>
    void statements() {
        statement();

        if (lexer.get_next_token() == Token::SEMICOLON) {
            get_next_token();

            statements();
        }
    }

    // <statement> → <ident><assignment_op><expression>
    void statement() {
        ident(true); // Adding identifier to symbol table

        get_next_token();

        expression();

        // END OF STATEMENT
        cout << current_statement.str() << endl;
        cout << "ID: " << ids << "; CONST: " << consts << "; OP: " << ops << ";" << endl;
        cout << (error_message.empty() ? "(OK)" : error_message) << endl;

        ids = consts = ops = 0;
        error_message.clear();
        current_statement.str("");
        current_statement.clear();
    }

    // <expression> → <term><term_tail>
    void expression() {
        term();
        term_tail();
    }

    // <term_tail> → <add_op><term><term_tail> | ε
    void term_tail() {
        if (lexer.get_next_token() == Token::ADD_OP) {
            ops++;
            get_next_token();

            term();
            term_tail();
        }
    }

    // <term> → <factor><factor_tail>
    void term() {
        factor();
        factor_tail();
    }

    // <factor_tail> → <mult_op><factor><factor_tail> | ε
    void factor_tail() {
        if (lexer.get_next_token() == Token::MULT_OP || lexer.get_next_token() == Token::DIV_OP) {
            ops++;
            get_next_token();

            factor();
            factor_tail();
        }
    }

    // <factor> → <left_paren><expression><right_paren> | <ident> | <const>
    void factor() {
        if (lexer.get_next_token() == Token::LEFT_PAREN) {
            get_next_token();

            expression();

            if (lexer.get_next_token() == Token::RIGHT_PAREN) {
                get_next_token();
            }
            else {
                error_message = "(ERROR) expected a right parenthesis";
            }
        }
        else if (lexer.get_next_token() == Token::IDENT) {
            ident();
        }
        else if (lexer.get_next_token() == Token::CONST) {
            consts++;
            get_next_token();
        }
        else {
            if (lexer.get_next_token() == Token::ADD_OP || lexer.get_next_token() == Token::MULT_OP) {
                error_message = "(WARNING) Ignoring extra operator";
                get_next_token();
            }
            else
                cerr << "(ERROR) expected a left parenthesis, an identifier, or a constant" << endl;
        }
    }

    // Method to process an identifier (following C identifier rules)
    void ident(bool add = false) {
        ids++;

        auto find_symbol = [this](const string &ident) {
            for (auto &symbol : symbol_table) {
                if (symbol.first == ident) {
                    return true;
                }
            }
            return false;
        };

        if (find_symbol(lexer.get_token_string()) == false) {
            if (add)
                symbol_table.emplace_back(lexer.get_token_string(), 0);
            else {
                cerr << "(Error) “정의되지 않은 변수(" << lexer.get_token_string() << ")가 참조됨";
                symbol_table.emplace_back(lexer.get_token_string(), nullopt);
            }
        }
        get_next_token();
    }

    void get_next_token() {
        lexer.lexical();
        string &&token = lexer.get_token_string();

        if (token == ";") {
            current_statement << "\b;";
        }
        else if (token != "EOF") {
            current_statement << lexer.get_token_string() << " ";
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    Parser parser(argv[1]);
    parser.parse();

    cin.get();

    return 0;
}