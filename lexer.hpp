/**
 * @file lexer.hpp
 *
 * @brief Declares the `CandidateToken` struct and the `Lexer` class.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */

#ifndef __TEMPLATE_HPP__
#define __TEMPLATE_HPP__

#include "token.hpp"

#include <list>
using namespace std;

/// @brief Represents a candidate for a token in the lexer.
struct CandidateToken
{
	/// @brief The type of token whose pattern was matched.
	const TokenType *tokenType;

	/// @brief The match of the program string with the token type's pattern.
	const smatch match;

	/// @brief Constructor.
	/// @param tokenType The type of token whose pattern was matched.
	/// @param match The match of the program string with the token type's
	/// pattern.
	CandidateToken(const TokenType *tokenType, smatch match);

	/// @brief Compare two candidate tokens based on their starting positions.
	/// Returns `true` if `a` starts before `b`.
	/// @param a A candidate token.
	/// @param b A candidate token.
	/// @return `true` if `a` starts before `b`, else `false`.
	static bool cmpPos(CandidateToken *&a, CandidateToken *&b);

#ifndef NDEBUG
	// get a string representation of this candidate token
	string to_string() const;
#endif
};

/// @brief Represents a lexer.
class Lexer
{
private:
	/// @brief Token types registered to the lexer.
	list<const TokenType *> tokenTypes;

	/// @brief Strings that have been processed by this lexer since it was
	/// created.+
	list<const string *> stringsLexed;

	/// @brief A list of candidate tokens that this lexer might choose to lex.
	list<const CandidateToken *> candidates;

	/// @brief A list of tokens lexed by this lexer.
	list<const BaseToken *> tokens;

public:
	/// @brief Register a token type with the lexer.
	/// @param tokenType The token type to register.
	void registerTokenType(const TokenType *tokenType);

	/// @brief Find all candidate tokens for a string and add them to the
	/// `candidates` list.
	/// @param s A string to lex.
	void findCandidates(const string *s);

	/// @brief Lex a string producing a list of `BaseToken` pointers.
	/// @param _s A string to lex.
	void lex(string _s);

	/// @brief Destructor.
	~Lexer();

#ifndef DEBUG
	/// @brief Get a string representation of the candidate tokens.
	/// @return A string representation of the candidate tokens.
	string candidatesStr() const;
#endif
};
#endif