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

TEST_FUNC(test_lexer_single_characters)
{
    Lexer *lex = new Lexer("(){}[];,:.?$@#");
    IS_TRUE(lex->Lex()->Type() == TT_LPAR, "Expected '('");
    IS_TRUE(lex->Lex()->Type() == TT_RPAR, "Expected ')'");
    IS_TRUE(lex->Lex()->Type() == TT_LBRACE, "Expected '{'");
    IS_TRUE(lex->Lex()->Type() == TT_RBRACE, "Expected '}'");
    IS_TRUE(lex->Lex()->Type() == TT_LBRACKET, "Expected '['");
    IS_TRUE(lex->Lex()->Type() == TT_RBRACKET, "Expected ']'");
    IS_TRUE(lex->Lex()->Type() == TT_SEMI, "Expected ';'");
    IS_TRUE(lex->Lex()->Type() == TT_COMMA, "Expected ','");
    IS_TRUE(lex->Lex()->Type() == TT_COLON, "Expected ':'");
    IS_TRUE(lex->Lex()->Type() == TT_DOT, "Expected '.'");
    IS_TRUE(lex->Lex()->Type() == TT_QUEST, "Expected '?'");
    IS_TRUE(lex->Lex()->Type() == TT_DOLLAR, "Expected '$'");
    IS_TRUE(lex->Lex()->Type() == TT_AT, "Expected '@'");
    IS_TRUE(lex->Lex()->Type() == TT_HASH, "Expected '#'");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_unexpected_character)
{
    Lexer *lex = new Lexer("`");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Unexpected character '`'", "Character '`' is unexpected");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_singleline_comments)
{
    Lexer *lex = new Lexer("// Un Commentaire français\n");
    IS_TRUE(lex->Lex()->Type() == TT_EOF, "The lexer do not read successfully the single-line comment");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_multiline_comments)
{
    Lexer *lex = new Lexer("/*Il\nétait\n\tune\nfois*/");
    IS_TRUE(lex->Lex()->Type() == TT_EOF, "The lexer do not read successfully the multi-line comment");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_comments)
{
    Lexer *lex = new Lexer("// Little comment\n/*Big\nComment\n\t..*/");
    IS_TRUE(lex->Lex()->Type() == TT_EOF, "The lexer do not read successfully multi and single line comments");

    TEST_SUCCEED();
}

int main(int argc, char **argv)
{
    ADD_TEST("test space", test_lexer_space);
    ADD_TEST("test empty code", test_lexer_empty_code);
    ADD_TEST("test single characters", test_lexer_single_characters);
    ADD_TEST("test unexpected character", test_lexer_unexpected_character);
    ADD_TEST("test single-line comments", test_lexer_singleline_comments);
    ADD_TEST("test multi-line comments", test_lexer_multiline_comments);
    ADD_TEST("test comments", test_lexer_comments);

    LAUNCH_TESTS("Test Lexer");
}