#ifndef KDPARK0723_PARSER_LEXER_H
#define KDPARK0723_PARSER_LEXER_H

#include <vector>
// #include <utility>
#include <string>

namespace Parser
{

template <typename Token_type>
class Lexer
{
public:
    using Token = std::pair<std::string, Token_type>;
    using Token_pattern = std::vector<Token>;
    using Tokens = std::vector<Token>;

    Lexer(std::string content, Token_pattern& pattern);
    Lexer(std::string content, Token& token);
    ~Lexer();

    void add_token(Token& token);
    bool remove_token(Token& token);
    bool remove_token(Token_type token_type);

    bool scan();
    void reset();
    Tokens get_tokkens();
    std::string get_error();
private:
    std::string content;
    std::string error_message;
    Token_pattern token_pattern;
    Tokens result_token;
    std::string regex_str;
    bool is_success;

    void cal_regex();
    void cal_regex(std::string regex_str);
};

} // Parser

#include "lexer.hpp"
#endif // !KDPARK0723_PARSER_LEXER_HPP