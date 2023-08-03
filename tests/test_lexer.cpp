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
    Lexer *lex = new Lexer(L"\n;\t;   ;/*abc\ndef\nghi*/");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Column() == 1 && tok->Line() == 1, L"Lexer position must be (l: 1, c: 1)");
    
    tok = lex->Lex();
    IS_TRUE(tok->Column() == 1 && tok->Line() == 2, L"Lexer position must be (l: 2, c: 1)");

    tok = lex->Lex();
    IS_TRUE(tok->Column() == 3 && tok->Line() == 2, L"Lexer position must be (l: 2, c: 3)");

    tok = lex->Lex();
    IS_TRUE(tok->Column() == 7 && tok->Line() == 2, L"Lexer position must be (l: 2, c: 7)");

    tok = lex->Lex();
    IS_TRUE(tok->Column() == 6 && tok->Line() == 4, L"Lexer position must be (l: 4, c: 6)");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF, L"Expected end-of-file");
    IS_TRUE(tok->Column() == 6 && tok->Line() == 4, L"Lexer position must be (l: 4, c: 6)");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_space)
{
    Lexer *lex = new Lexer(L" \t\v");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF, L"The lexer does not understand ' ' '\\t' and/or '\\v' as spaces");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_empty_code)
{
    Lexer *lex = new Lexer(L"");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF, L"Empty code should return EOF token");
    
    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_single_characters)
{
    Lexer *lex = new Lexer(L"\n(){}[];,:.?$@#");
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE, L"Expected '\\n'");
    IS_TRUE(lex->Lex()->Type() == TT_LPAR, L"Expected '('");
    IS_TRUE(lex->Lex()->Type() == TT_RPAR, L"Expected ')'");
    IS_TRUE(lex->Lex()->Type() == TT_LBRACE, L"Expected '{'");
    IS_TRUE(lex->Lex()->Type() == TT_RBRACE, L"Expected '}'");
    IS_TRUE(lex->Lex()->Type() == TT_LBRACKET, L"Expected '['");
    IS_TRUE(lex->Lex()->Type() == TT_RBRACKET, L"Expected ']'");
    IS_TRUE(lex->Lex()->Type() == TT_SEMI, L"Expected ';'");
    IS_TRUE(lex->Lex()->Type() == TT_COMMA, L"Expected ','");
    IS_TRUE(lex->Lex()->Type() == TT_COLON, L"Expected ':'");
    IS_TRUE(lex->Lex()->Type() == TT_DOT, L"Expected '.'");
    IS_TRUE(lex->Lex()->Type() == TT_QUEST, L"Expected '?'");
    IS_TRUE(lex->Lex()->Type() == TT_DOLLAR, L"Expected '$'");
    IS_TRUE(lex->Lex()->Type() == TT_AT, L"Expected '@'");
    IS_TRUE(lex->Lex()->Type() == TT_HASH, L"Expected '#'");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_complex_characters)
{
    Lexer *lex = new Lexer(L"+++=+-*---=**=**<<<<=");
    
    IS_TRUE(lex->Lex()->Type() == TT_PLUSPLUS, L"Expected '++'");
    IS_TRUE(lex->Lex()->Type() == TT_PLUSEQ, L"Expected '+='");
    IS_TRUE(lex->Lex()->Type() == TT_PLUS, L"Expected '+'");
    IS_TRUE(lex->Lex()->Type() == TT_MINUS, L"Expected '-'");
    IS_TRUE(lex->Lex()->Type() == TT_STAR, L"Expected '*'");
    IS_TRUE(lex->Lex()->Type() == TT_MINUSMINUS, L"Expected '--'");
    IS_TRUE(lex->Lex()->Type() == TT_MINUSEQ, L"Expected '-='");
    IS_TRUE(lex->Lex()->Type() == TT_POWEREQ, L"Expected '**='");
    IS_TRUE(lex->Lex()->Type() == TT_POWER, L"Expected '**'");
    IS_TRUE(lex->Lex()->Type() == TT_LSHIFT, L"Expected '<<'");
    IS_TRUE(lex->Lex()->Type() == TT_LSHIFTEQ, L"Expected '<<='");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_unexpected_character)
{
    Lexer *lex = new Lexer(L"`§");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == L"Unexpected character '`'", L"Character '`' is unexpected");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == L"Unexpected character '§'", L"Character '§' is unexpected");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_singleline_comments)
{
    Lexer *lex = new Lexer(L"// Un Commentaire français\n");
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE, L"The lexer do not read successfully the single-line comment");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_multiline_comments)
{
    Lexer *lex = new Lexer(L"/*Il\nétait\n\tune\nfois*/");
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE && lex->Lex()->Type() == TT_EOF, L"The lexer do not read successfully the multi-line comment");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_comments)
{
    Lexer *lex = new Lexer(L"// Little comment\n/*Big\nComment\n\t..*/");
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE, L"The lexer do not read successfully single-line comments");
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE && lex->Lex()->Type() == TT_EOF, L"The lexer do not read successfully multi-line comments");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_special_keywords)
{
    Lexer *lex = new Lexer(L"null in is false true break continue");
    IS_TRUE(lex->Lex()->Type() == TT_NULL, L"Expected 'null'");
    IS_TRUE(lex->Lex()->Type() == TT_IN, L"Expected 'in'");
    IS_TRUE(lex->Lex()->Type() == TT_IS, L"Expected 'is'");

    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BOOLEAN && tok->Text() == L"false", L"Expected 'false'");
    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BOOLEAN && tok->Text() == L"true", L"Expected 'true'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_CONTROLLER && tok->Text() == L"break", L"Expected 'break'");
    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_CONTROLLER && tok->Text() == L"continue", L"Expected 'continue'");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_words)
{
    Lexer *lex = new Lexer(L"word __word word__ two-word");
    Token *tok;

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == L"word", L"Expected word 'word'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == L"__word", L"Expected word '__word'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == L"word__", L"Expected word 'word__'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == L"two", L"Expected word 'two'");
    IS_TRUE(lex->Lex()->Type() == TT_MINUS, L"Expected char '-'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == L"word", L"Expected word 'word'");
    IS_TRUE(lex->Lex()->Type() == TT_EOF, L"Expected end-of-file");

    TEST_SUCCEED();
}

int main(int argc, char **argv)
{
    ADD_TEST(L"test position", test_lexer_position);
    ADD_TEST(L"test space", test_lexer_space);
    ADD_TEST(L"test empty code", test_lexer_empty_code);
    ADD_TEST(L"test single characters", test_lexer_single_characters);
    ADD_TEST(L"test complex characters", test_lexer_complex_characters);
    ADD_TEST(L"test unexpected character", test_lexer_unexpected_character);
    ADD_TEST(L"test single-line comments", test_lexer_singleline_comments);
    ADD_TEST(L"test multi-line comments", test_lexer_multiline_comments);
    ADD_TEST(L"test comments", test_lexer_comments);
    ADD_TEST(L"test special keywords", test_lexer_special_keywords);
    ADD_TEST(L"test words", test_lexer_words);

    LAUNCH_TESTS(L"Test Lexer");
}