#include "../include/lexer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>

using namespace Parser;

enum class Token_type
{
    literal_true,     // the `true` literal
    literal_false,    // the `false` literal
    literal_null,     // the `null` literal
    value_string,     // a string -- use get_string() for actual value
    value_number,     // a number
    begin_array,      // the character for array begin `[`
    begin_object,     // the character for object begin `{`
    end_array,        // the character for array end `]`
    end_object,       // the character for object end `}`
    name_separator,   // the name separator `:`
    value_separator,  // the value separator `,`
    end_of_input,     // indicating the end of the input buffer
};

std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    return(contents.str());
  }
  throw(errno);
}

int main()
{
    std::string inFileName = "sample.json";
    std::string src = get_file_contents(inFileName.c_str());
    
    Lexer<Token_type>::Token_pattern pattern
    {
        { "true", Token_type::literal_true },
        { "false", Token_type::literal_false },
        { "null", Token_type::literal_null },
        { R"("(?:[^"\\]|\\(?:"|\\|\/|b|f|n|r|t|u(?:[a-f]|[A-F]|[0-9]){4}))*")", Token_type::value_string },
        { R"(-?(?:0|(?:[1-9][0-9]*))(?:.[0-9]+)?(?:(?:e|E)(?:\+|-)[0-9]+)?)", Token_type::value_number },
        { R"(\[)", Token_type::begin_array },
        { R"(\{)", Token_type::begin_object },
        { R"(\])", Token_type::end_array },
        { R"(\})", Token_type::end_object },
        { R"(:)", Token_type::name_separator },
        { R"(,)", Token_type::value_separator },
        { R"(\x00)", Token_type::end_of_input },
    };
    std::map<Token_type, std::string> name
    {
        { Token_type::literal_true, "literal_true" },     
        { Token_type::literal_false, "literal_false" },    
        { Token_type::literal_null, "literal_null" },     
        { Token_type::value_string, "value_string" },     
        { Token_type::value_number, "value_number" },     
        { Token_type::begin_array, "begin_array" },      
        { Token_type::begin_object, "begin_object" },     
        { Token_type::end_array, "end_array" },        
        { Token_type::end_object, "end_object" },       
        { Token_type::name_separator, "name_separator" },   
        { Token_type::value_separator, "value_separator" },  
        { Token_type::end_of_input, "end_of_input" },    
    };

    Lexer<Token_type> lexer{ src, pattern };

    if (lexer.scan())
    {
        const auto tokens = lexer.get_tokkens();

        for (auto& i : tokens)
           std::cout << i.first << "\t\t" << name[i.second] << std::endl;
    }
    else
    {
        auto error = lexer.get_error();
        std::cout << error << std::endl;
    }
    
    return 0;
}