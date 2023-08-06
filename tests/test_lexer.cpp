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
    Lexer *lex = new Lexer("\n;\t;   ;/*abc\ndef\nghi*/ \"é\nà\"");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Column() == 1 && tok->Line() == 1, "Lexer position must be (l: 1, c: 1)");
    
    tok = lex->Lex();
    IS_TRUE(tok->Column() == 1 && tok->Line() == 2, "Lexer position must be (l: 2, c: 1)");

    tok = lex->Lex();
    IS_TRUE(tok->Column() == 3 && tok->Line() == 2, "Lexer position must be (l: 2, c: 3)");

    tok = lex->Lex();
    IS_TRUE(tok->Column() == 7 && tok->Line() == 2, "Lexer position must be (l: 2, c: 7)");

    tok = lex->Lex();
    IS_TRUE(tok->Column() == 6 && tok->Line() == 4, "Lexer position must be (l: 4, c: 6)");

    tok = lex->Lex();
    IS_TRUE(tok->Column() == 7 && tok->Line() == 4, "Lexer position must be (l: 4, c: 7)");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF, "Expected end-of-file");
    IS_TRUE(tok->Column() == 3 && tok->Line() == 5, "Lexer position must be (l: 5, c: 3)");

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

TEST_FUNC(test_lexer_complex_characters)
{
    Lexer *lex = new Lexer("+++=+-*---=**=**<<<<=");
    
    IS_TRUE(lex->Lex()->Type() == TT_PLUSPLUS, "Expected '++'");
    IS_TRUE(lex->Lex()->Type() == TT_PLUSEQ, "Expected '+='");
    IS_TRUE(lex->Lex()->Type() == TT_PLUS, "Expected '+'");
    IS_TRUE(lex->Lex()->Type() == TT_MINUS, "Expected '-'");
    IS_TRUE(lex->Lex()->Type() == TT_STAR, "Expected '*'");
    IS_TRUE(lex->Lex()->Type() == TT_MINUSMINUS, "Expected '--'");
    IS_TRUE(lex->Lex()->Type() == TT_MINUSEQ, "Expected '-='");
    IS_TRUE(lex->Lex()->Type() == TT_POWEREQ, "Expected '**='");
    IS_TRUE(lex->Lex()->Type() == TT_POWER, "Expected '**'");
    IS_TRUE(lex->Lex()->Type() == TT_LSHIFT, "Expected '<<'");
    IS_TRUE(lex->Lex()->Type() == TT_LSHIFTEQ, "Expected '<<='");

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
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE, "The lexer do not read successfully the single-line comment");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_multiline_comments)
{
    Lexer *lex = new Lexer("/*Il\nétait\n\tune\nfois*/");
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE && lex->Lex()->Type() == TT_EOF, "The lexer do not read successfully the multi-line comment");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_comments)
{
    Lexer *lex = new Lexer("// Little comment\n/*Big\nComment\n\t..*/");
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE, "The lexer do not read successfully single-line comments");
    IS_TRUE(lex->Lex()->Type() == TT_NEW_LINE && lex->Lex()->Type() == TT_EOF, "The lexer do not read successfully multi-line comments");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_special_keywords)
{
    Lexer *lex = new Lexer("null in is as false true break continue");
    IS_TRUE(lex->Lex()->Type() == TT_NULL, "Expected 'null'");
    IS_TRUE(lex->Lex()->Type() == TT_IN, "Expected 'in'");
    IS_TRUE(lex->Lex()->Type() == TT_IS, "Expected 'is'");
    IS_TRUE(lex->Lex()->Type() == TT_AS, "Expected 'as'");

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
    Lexer *lex = new Lexer("word __word word__ two-word word1 2word");
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
    
    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == "word1", "Expected word 'word1'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_INTEGER, "Expected number between 'word1' and '2word'");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_WORD && tok->Text() == "word", "Expected word 'word'");
    IS_TRUE(lex->Lex()->Type() == TT_EOF, "Expected end-of-file");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_strings)
{
    Lexer *lex = new Lexer("\"\\xe282ac\" \"平仮名, ひらがな\n한\t글\"");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_STRING && tok->Text() == "€", "Incorrect string n°1");
    IS_TRUE(tok->Line() == 1 && tok->Column() == 1, "Incorrect string n°1 position");
    
    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_STRING && tok->Text() == "平仮名, ひらがな한글", "Incorrect string n°2");
    IS_TRUE(tok->Line() == 1 && tok->Column() == 12, "Incorrect string n°2 position");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF && tok->Line() == 2 && tok->Column() == 5, "Expected end-of-file at position (l: 2, c: 5)");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_characters)
{
    Lexer *lex = new Lexer("'²' '\\uF09F9880' '\\n'");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_CHAR && tok->Text() == "²", "Incorrect character n°1");
    IS_TRUE(tok->Line() == 1 && tok->Column() == 1, "Incorrect character n°1 position");
    
    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_CHAR && tok->Text() == "😀", "Incorrect character n°2");
    IS_TRUE(tok->Line() == 1 && tok->Column() == 5, "Incorrect character n°2 position");
    
    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_CHAR && tok->Text() == "\n", "Incorrect character n°3");
    IS_TRUE(tok->Line() == 1 && tok->Column() == 18, "Incorrect character n°3 position");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF && tok->Line() == 1 && tok->Column() == 22, "Expected end-of-file at position (l: 1, c: 22)");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_unterminated_string)
{
    Lexer *lex = new Lexer("\"abc\ndef 123");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Unterminated string", "Expected an unterminated string BAD TOKEN");
    IS_TRUE(tok->Line() == 1 && tok->Column() == 1, "Unterminated string is at position (l: 1, c: 1)");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF && tok->Line() == 2 && tok->Column() == 8, "Expected end-of-file at position (l: 2, c: 8)");
    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_unterminated_character)
{
    Lexer *lex = new Lexer("'too big\nfor a simple char");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Unterminated character", "Expected an unterminated character BAD TOKEN");
    IS_TRUE(tok->Line() == 1 && tok->Column() == 1, "Unterminated character is at position (l: 1, c: 1)");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_EOF && tok->Line() == 2 && tok->Column() == 18, "Expected end-of-file at position (l: 2, c: 18)");
    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_numbers)
{
    Lexer *lex = new Lexer("123 0x123 0o123 0b1011 12.345 1e+87 0b1011_1011 1_2e-12_3 12L 12.34D 12D 1E+2D 1.2e+2");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_INTEGER, "Expected that 123 is an integer");
    IS_TRUE(tok->Text() == "123", "First number is 123");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_HEXADECIMAL, "Expected that 123 is an hexadecimal");
    IS_TRUE(tok->Text() == "123", "Second number is 123");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_OCTAL, "Expected that 123 is an octal");
    IS_TRUE(tok->Text() == "123", "Third number is 123");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BINARY, "Expected that 1011 is a binary number");
    IS_TRUE(tok->Text() == "1011", "Fourth number is 1011");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_DECIMAL, "Expected that 12.345 is a decimal number");
    IS_TRUE(tok->Text() == "12.345", "Fifth number is 12.345");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_DECIMAL, "Expected that 1e+87 is a decimal number");
    IS_TRUE(tok->Text() == "1e+87", "Sixth number is 1e+87");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BINARY, "Expected that 10111011 is a binary number");
    IS_TRUE(tok->Text() == "10111011", "Seventh number is 10111011");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_DECIMAL, "Expected that 12e-123 is a decimal number");
    IS_TRUE(tok->Text() == "12e-123", "Eighth number is 12e-123");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_LONG_INTEGER, "Expected that 12 is a long integer number");
    IS_TRUE(tok->Text() == "12", "Ninth number is 12");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_LONG_DECIMAL, "Expected that 12.34 is a long decimal number");
    IS_TRUE(tok->Text() == "12.34", "Tenth number is 12.34");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_LONG_DECIMAL, "Expected that 12 is a long decimal number");
    IS_TRUE(tok->Text() == "12", "Eleventh number is 12");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_LONG_DECIMAL, "Expected that 1e+2 is a long decimal number");
    IS_TRUE(tok->Text() == "1e+2", "Twelfth number is 1E+2");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_DECIMAL, "Expected that 1.2e+2 is a decimal number");
    IS_TRUE(tok->Text() == "1.2e+2", "Twelfth number is 1.2e+2");

    TEST_SUCCEED();
}

TEST_FUNC(test_lexer_incorrect_numbers)
{
    Lexer *lex = new Lexer("12.3L 1e+23L 12e23 0x12E-3 0o123.45 12e+34e+56 12.34.56 1e+ 12.");
    Token *tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Decimal number can not have modifier 'L'", "Expected that 12.3L is not correct");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Decimal number can not have modifier 'L'", "Expected that 1e+23L is not correct");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Exponential number must have '+' or '-' after 'e'", "Expected that 12e23 is not correct");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Hexadecimal, octal or binary number can not be exponential number", "Expected that 0x12E-3 is not correct");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Hexadecimal, octal or binary number can not be decimal number", "Expected that 0o123.45 is not correct");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Exponential number can not have more than one 'e'", "Expected that 12e+34e+56 is not correct");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Decimal number can not have more than one '.'", "Expected that 12.34.56 is not correct");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Exponential number must have at least 1 digit after 'e+' or 'e-'", "Expected that 1e+ is not correct");

    tok = lex->Lex();
    IS_TRUE(tok->Type() == TT_BAD && tok->Text() == "Decimal number must have at least 1 digit after '.'", "Expected that 12. is not correct");

    TEST_SUCCEED();
}

void test_lexer_complete_code_1()
{
    FILE *test_file = fopen64("tests/test_file.sbw", "rb");
    if (!test_file) return;

    sbw_string code;
    char c = fgetc(test_file);
    while (c != -1) {
        code += c;
        c = fgetc(test_file);
    }
    fclose(test_file);

    Lexer *lex = new Lexer(code);
    Token *tok = lex->Lex();
    do {
        sbw_print(9, "TOKEN-", std::to_string(tok->Type()).c_str(), " => ", tok->Text().c_str(), " (l: ", std::to_string(tok->Line()).c_str(), " c: ", std::to_string(tok->Column()).c_str(), ")\n");
        tok = lex->Lex();
    } while (tok->Type() != TT_EOF);
}

int main(int argc, char **argv)
{
    sbw_print(1, "---\n");
    test_lexer_complete_code_1();
    sbw_print(1, "\n---\n\n");

    INIT_TEST();

    ADD_TEST("test position", test_lexer_position);
    ADD_TEST("test space", test_lexer_space);
    ADD_TEST("test empty code", test_lexer_empty_code);
    ADD_TEST("test single characters", test_lexer_single_characters);
    ADD_TEST("test complex characters", test_lexer_complex_characters);
    ADD_TEST("test unexpected character", test_lexer_unexpected_character);
    ADD_TEST("test single-line comments", test_lexer_singleline_comments);
    ADD_TEST("test multi-line comments", test_lexer_multiline_comments);
    ADD_TEST("test comments", test_lexer_comments);
    ADD_TEST("test special keywords", test_lexer_special_keywords);
    ADD_TEST("test words", test_lexer_words);
    ADD_TEST("test strings", test_lexer_strings);
    ADD_TEST("test characters", test_lexer_characters);
    ADD_TEST("test unterminated string", test_lexer_unterminated_string);
    ADD_TEST("test unterminated character", test_lexer_unterminated_character);
    ADD_TEST("test numbers", test_lexer_numbers);
    ADD_TEST("test incorrect numbers", test_lexer_incorrect_numbers);

    LAUNCH_TESTS("Test Lexer");
}