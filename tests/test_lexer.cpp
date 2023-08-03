#include "core/lexer.hpp"
#include "TEST.hpp"

TEST_FUNC(test_lexer_space)
{
    Lexer *lex = new Lexer(" \t\n");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF, "The lexer does not understand ' ' '\\t' and/or '\\n' as spaces");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_empty_code)
{
    Lexer *lex = new Lexer("");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF, "Empty code should return EOF token");
    
    TEST_SUCCEED();
}

int main(int argc, char **argv)
{
    ADD_TEST("test lexer space", test_lexer_space);
    ADD_TEST("test lexer empty code", test_lexer_empty_code);

    LAUNCH_TESTS("Test Lexer");
}