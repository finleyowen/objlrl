/**
 * @file lexer.hpp
 *
 * @brief Declares the `Lexer` class.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */

#ifndef __TEMPLATE_HPP__
#define __TEMPLATE_HPP__

#include "token.hpp"

#include <list>
using namespace std;

/// @brief Represents a lexer.
class Lexer
{
private:
    /// @brief Token types registered to the lexer.
    list<const TokenType *> tokenTypes;

    /// @brief Strings that have been processed by this lexer since it was
    /// created.
    list<const string *> stringsLexed;

    /// @brief A list of candidate tokens that this lexer might choose to lex.
    list<const CandidateToken *> candidates;

    /// @brief A list of tokens lexed by this lexer.
    list<const BaseToken *> tokens;

protected:
    /// @brief Function to handle unmatched input.
    /// @param s String representation of the program where the unmatched input
    /// was found.
    /// @param position Position of the unmatched input in the string `s`.
    /// @param length Length of the unmatched input in the string `s`.
    void handleUnmatched(const string *s, unsigned int position,
                         unsigned int length);

public:
    /// @brief Register a token type with the lexer.
    /// @param tokenType The token type to register.
    void registerTokenType(const TokenType *tokenType);

    /// @brief Find all candidate tokens for a string and add them to the
    /// `candidates` list.
    /// @param s A string to lex.
    void findCandidates(const string *s);

    /// @brief Sort the candidates list by their starting positions in the
    /// program.
    void sortCandidates();

    /// @brief Filter out overlapping candidates from the list.
    void filterCandidates(const string *s);

    /// @brief Lex a string producing a list of `BaseToken` pointers.
    /// @param _s A string to lex.
    void lex(string _s);

    /// @brief Destructor.
    ~Lexer();

#ifndef DEBUG
    /// @brief Get a string representation of the candidate tokens (debug only).
    /// @return A string representation of the candidate tokens.
    string candidatesString() const;

    /// @brief Get a string representation of the tokens stored in the lexer
    /// (debug only).
    /// @return A string representation of the tokens stored in the lexer.
    string tokensString() const;
#endif
};
#endif