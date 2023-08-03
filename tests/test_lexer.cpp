/**
 * @file test_lexer.cpp
 * @author LucaStarz
 * @brief Tests for the seabow's lexer.
 * @date 2023-08-03
 * 
 * @copyright Copyright (c) 2023
*/

#include "core/lexer.hpp"
#include "TEST.hpp"

TEST_FUNC(test_lexer_position)
{
    Lexer *lex = new Lexer("\n;");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Column() == 1 && tok->Line() == 1, "Lexer position must be (l: 1, c: 1)");
    
    tok = lex->Lex();
    IS_TRUE(tok->Column() == 2 && tok->Line() == 1, "Lexer position must be (l: 2, c: 1)");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_space)
{
    Lexer *lex = new Lexer(" \t\v");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF, "The lexer does not understand ' ' '\\t' and/or '\\v' as spaces");

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
    Lexer *lex = new Lexer("\n(){}[];,:.?$@#");
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE, "Expected '\\n'");
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
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE, "The lexer do not read successfully single-line comments");
    IS_TRUE(lex->Lex()->Type() == TT_EOF, "The lexer do not read successfully multi-line comments");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_special_keywords)
{
    Lexer *lex = new Lexer("null in is false true break continue");
    IS_TRUE(lex->Lex()->Type() == TT_NULL, "Expected 'null'");
    IS_TRUE(lex->Lex()->Type() == TT_IN, "Expected 'in'");
    IS_TRUE(lex->Lex()->Type() == TT_IS, "Expected 'is'");

    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BOOLEAN && tok->Text() == "false", "Expected 'false'");
    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BOOLEAN && tok->Text() == "true", "Expected 'true'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_CONTROLLER && tok->Text() == "break", "Expected 'break'");
    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_CONTROLLER && tok->Text() == "continue", "Expected 'continue'");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_words)
{
    Lexer *lex = new Lexer("word __word word__ two-word");
    Token *tok;

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == "word", "Expected word 'word'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == "__word", "Expected word '__word'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == "word__", "Expected word 'word__'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == "two", "Expected word 'two'");
    IS_TRUE(lex->Lex()->Type() == TT_MINUS, "Expected char '-'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == "word", "Expected word 'word'");
    IS_TRUE(lex->Lex()->Type() == TT_EOF, "Expected end-of-file");

    TEST_SUCCEED();
}

int main(int argc, char **argv)
{
    ADD_TEST("test position", test_lexer_position);
    ADD_TEST("test space", test_lexer_space);
    ADD_TEST("test empty code", test_lexer_empty_code);
    ADD_TEST("test single characters", test_lexer_single_characters);
    ADD_TEST("test unexpected character", test_lexer_unexpected_character);
    ADD_TEST("test single-line comments", test_lexer_singleline_comments);
    ADD_TEST("test multi-line comments", test_lexer_multiline_comments);
    ADD_TEST("test comments", test_lexer_comments);
    ADD_TEST("test special keywords", test_lexer_special_keywords);
    ADD_TEST("test words", test_lexer_words);

    LAUNCH_TESTS("Test Lexer");
}