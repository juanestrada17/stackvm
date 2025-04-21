#include <iostream>
#include <vector>
#include <cstdint>

typedef uint8_t byte;
typedef std::vector<std::string> strings;

// enumeration of states 
enum State : byte 
{
    START, 
    READCHAR, 
    READBLOCK, 
    SKIP, 
    DUMP, 
    COMMENT, 
    END
};

class Lexer
{
    bool my_isspace(char c);
    bool isspecial(char c);
    bool isgroup(char c);
    char end_char, beg_char;
    public:
    strings lex(std::string s);
};