#ifndef KDPARK0723_PARSER_LEXER_HPP
#define KDPARK0723_PARSER_LEXER_HPP

#include "lexer.h"

#include <regex>
#include <iterator>

template<typename Token_type>
Parser::Lexer<Token_type>::Lexer(std::string content, Parser::Lexer<Token_type>::Token_pattern & pattern)
    : content{ content }, token_pattern{ pattern }, result_token{ }, regex_str{ "" }, is_success{ false }
{
    cal_regex();
}

template<typename Token_type>
Parser::Lexer<Token_type>::Lexer(std::string content, Parser::Lexer<Token_type>::Token & token)
    : content{ content }, token_pattern{ }, result_token{ }, regex_str{ "" }, is_success{ false }
{
    add_token(token);
}

template<typename Token_type>
Parser::Lexer<Token_type>::~Lexer()
{
}

template<typename Token_type>
void Parser::Lexer<Token_type>::add_token(Parser::Lexer<Token_type>::Token & token)
{
    token_pattern.push_back(token);
    cal_regex(token.first);
}

template<typename Token_type>
bool Parser::Lexer<Token_type>::remove_token(Parser::Lexer<Token_type>::Token & token)
{
    return false;
}

template<typename Token_type>
bool Parser::Lexer<Token_type>::remove_token(Token_type token_type)
{
    return false;
}

template<typename Token_type>
bool Parser::Lexer<Token_type>::scan()
{
    std::regex re{regex_str, std::regex_constants::ECMAScript};
    auto words_begin = std::sregex_iterator(content.begin(), content.end(), re);
    auto words_end = std::sregex_iterator();
    
    for (auto it = words_begin; it != words_end; ++it)
    {
        std::size_t index = 1;

        for (; index < it->size()+1; ++index)
            if (!it->str(index).empty()) // determine which submatch was matched
                break;
    
        result_token.push_back(Token{ it->str(), token_pattern[index-1].second });
    }
    return is_success = true;
}

template<typename Token_type>
void Parser::Lexer<Token_type>::reset()
{
}

template<typename Token_type>
typename Parser::Lexer<Token_type>::Tokens Parser::Lexer<Token_type>::get_tokkens()
{
    return result_token;
}

template<typename Token_type>
std::string Parser::Lexer<Token_type>::get_error()
{
    return std::string();
}

template<typename Token_type>
void Parser::Lexer<Token_type>::cal_regex()
{
    regex_str = "";
    for (auto const& x : token_pattern)
        regex_str += "(" + x.first + ")|"; // parenthesize the submatches
    regex_str.pop_back();
}

template<typename Token_type>
void Parser::Lexer<Token_type>::cal_regex(std::string regex_str)
{
    this.regex_str += "|(" + regex_str + ")";
}

#endif //!KDPARK0723_PARSER_LEXER_HPP