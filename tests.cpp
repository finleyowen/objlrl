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
#include <sstream>
#include <cassert>
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

// check the behaviour of the lexer when handling overlapping token types
void lexer_test1(void)
{
    // create a lexer and register token types
    Lexer *lexer = new Lexer();
    lexer->registerTokenType(&WhitespaceToken::tokenType);
    lexer->registerTokenType(&UIntToken::tokenType);
    lexer->registerTokenType(&IntToken::tokenType);

    // lex program string
    string s = "12 -24 65 -2 44 -67";
    lexer->lex(s);

    string candidatesString = lexer->candidatesString();
    istringstream ss = istringstream(candidatesString);

    // check program output
    string line;
    assert(getline(ss, line));
    assert(line == "uint token: \"12\"");
    assert(getline(ss, line));
    assert(line == "whitespace token: \" \"");
    assert(getline(ss, line));
    assert(line == "int token: \"-24\"");
    assert(getline(ss, line));
    assert(line == "whitespace token: \" \"");
    assert(getline(ss, line));
    assert(line == "uint token: \"65\"");
    assert(getline(ss, line));
    assert(line == "whitespace token: \" \"");
    assert(getline(ss, line));
    assert(line == "int token: \"-2\"");
    assert(getline(ss, line));
    assert(line == "whitespace token: \" \"");
    assert(getline(ss, line));
    assert(line == "uint token: \"44\"");
    assert(getline(ss, line));
    assert(line == "whitespace token: \" \"");
    assert(getline(ss, line));
    assert(line == "int token: \"-67\"");

    // cleanup
    delete lexer;
}

// check the behaviour of the lexer when handling unmatched input
void lexer_test2()
{

    // create a lexer and register token types (note: don't register the
    // whitespace token)
    Lexer *lexer = new Lexer();
    lexer->registerTokenType(&UIntToken::tokenType);
    lexer->registerTokenType(&IntToken::tokenType);

    // program string
    string s = "12 -24 65 -2 44 -67";

    // note the expected behaviour is a runtime error
    bool behavedAsExpected;

    try
    {
        // lex program string
        lexer->lex(s);

        // uh oh - that should have thrown a runtime error since the spaces are
        // unmatched
        behavedAsExpected = false;
    }
    catch (runtime_error &e)
    {
        // yay - threw a runtime error when a runtime error was expected
        behavedAsExpected = true;
    }

    // throw a runtime error if the behaviour was unmatched - this is a little
    // confusing since we just handled a runtime error, but this error is thrown
    // if lexer->lex(s) does not throw a runtime error, and this error is not
    // handled in the code and will crash the test program
    if (!behavedAsExpected)
    {
        throw runtime_error("Didn't throw runtime error on unmatched input!");
    }

    delete lexer;
}

// program entry - run all tests and debug if required
int main(void)
{
    lexer_test1();
    lexer_test2();
    // lexer_debug();
}

//