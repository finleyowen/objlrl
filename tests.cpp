/**
 * @file tests.cpp
 *
 * @brief Tests for `objlex`.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */

#include "token.hpp"
#include "lexer.hpp"

#include <iostream>
using namespace std;

// unsigned integer token
class UIntToken : public BaseToken
{
public:
    const unsigned int val;

    UIntToken(unsigned int val) : val(val) {}

    // lex method
    static const BaseToken *lex(const smatch *match)
    {
        return new UIntToken(stoul(match->str(0)));
    }

    // token type getter (overrides pure virtual method in BaseToken class)
    const TokenType *getTokenType() const
    {
        return &tokenType;
    }

    // token type
    inline static const TokenType tokenType = TokenType("uint", "[0-9]+", lex);
};

// integer token
class IntToken : public BaseToken
{
public:
    const int val;

    IntToken(int val) : val(val) {}

    // lex method
    static const BaseToken *lex(const smatch *match)
    {
        string s = match->str(0);
        return new IntToken(stoi(s));
    }

    // token type getter
    const TokenType *getTokenType() const
    {
        return &tokenType;
    }

    // token type
    inline static const TokenType tokenType = TokenType("int", "-?[0-9]+", lex);
};

// whitespace token
class WhitespaceToken : public BaseToken
{
public:
    // lex method
    static const BaseToken *lex(const smatch *match)
    {
        return new WhitespaceToken();
    }

    // token type getter
    const TokenType *getTokenType() const
    {
        return &tokenType;
    }

    // token type
    inline static const TokenType tokenType =
        TokenType("whitespace", "[ \t\n]+", lex);
};

// help debug the lexer
void lexer_debug(void)
{
    // create a lexer and register token types
    Lexer *lexer = new Lexer();
    lexer->registerTokenType(&WhitespaceToken::tokenType);
    lexer->registerTokenType(&UIntToken::tokenType);
    lexer->registerTokenType(&IntToken::tokenType);

    // lex program string
    string s = "12 -24 65 -2 44 -67";
    lexer->lex(s);

    // print the candidate tokens
    cout << "== candidates ==\n";
    cout << lexer->candidatesString() << "\n";

    // print the tokens
    cout << "== tokens ==\n";
    cout << lexer->tokensString() << "\n";

    // cleanup
    delete lexer;
}

int main(void)
{
    lexer_debug();
}

//