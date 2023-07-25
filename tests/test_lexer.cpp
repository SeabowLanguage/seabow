#include "core/lexer.hpp"
#include <gtest/gtest.h>

TEST(test_lexer_space, space_1) {
    Lexer *l = new Lexer(" \n\t");
    Token *t = l->Lex();

    ASSERT_TRUE(t->Type() == TT_EOF);

    delete t;
    delete l;
}

TEST(test_lexer_character, left_parenthesis) {
    Lexer *l = new Lexer("(");
    Token *t = l->Lex();

    ASSERT_TRUE(t->Type() == TT_LPAR);

    delete t;
    delete l;
}

TEST(test_lexer_character, right_parenthesis) {
    Lexer *l = new Lexer(")");
    Token *t = l->Lex();

    ASSERT_TRUE(t->Type() == TT_RPAR);

    delete t;
    delete l;
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}