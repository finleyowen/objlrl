/**
 * @file token.hpp
 *
 * @brief Declares the `TokenType` struct, the `BaseToken` class, the
 * `CandidateToken` struct, and the `TokenQueue` class.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */

#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <string>
#include <functional>
#include <regex>
#include <list>
using namespace std;

// declare the BaseToken up here to use it in the TokenType declaration; then
// redeclare it below with attributes and methods
class BaseToken;

/// @brief Represents a type of token in the language.
struct TokenType
{
	/// @brief Name for this token type.
	const string name;

	/// @brief Regular expression (as a string) that matches tokens of this
	/// token type.
	const string pat;

	/// @brief Function to lex tokens of this token type.
	const function<const BaseToken *(const smatch *)> lexFn;

	/// @brief Constructor.
	/// @param name Name for this token type.
	/// @param pat Regular expression (as a string) that matches tokens of this
	/// token type.
	/// @param lexFn Function to lex tokens of this token type.
	TokenType(string name, string pat,
			  function<const BaseToken *(const smatch *)> lexFn);

	/// @brief Convenience method to call the `lexFn` function member.
	/// @param match The match to call `lexFn` on.
	/// @return The output of the `lexFn` function; a token.
	const BaseToken *lex(const smatch *match) const;

#ifndef NDEBUG
	/// @brief Get a string representation of the token type (debug only).
	/// @return A string representation of the token type.
	string toString() const;
#endif
};

/// @brief Represents a token in the program.
class BaseToken
{
protected:
	/// @brief Get the token type associated with this token.
	/// @return Pointer to a `TokenType`.
	/// @note Concrete subclasses need to override this pure virtual method.
	virtual const TokenType *getTokenType() const = 0;

public:
	/// @brief Virtual destructor.
	virtual ~BaseToken();

#ifndef NDEBUG
	string toString() const;
#endif
};

/// @brief Represents a candidate for a token in the lexer.
struct CandidateToken
{
	/// @brief The type of token whose pattern was matched. This should point to
	/// an `inline static const TokenType` attribute in the token class (a child
	/// class of `BaseToken`).
	const TokenType *tokenType;

	/// @brief The match of the program string with the token type's pattern.
	const smatch match;

	const string *src;

	/// @brief Constructor.
	/// @param tokenType The type of token whose pattern was matched.
	/// @param match The match of the program string with the token type's
	/// pattern.
	CandidateToken(const TokenType *tokenType, const smatch match,
				   const string *src);

	/// @brief Compare two candidate tokens by their starting positions.
	/// Returns `true` if `a` starts before `b`.
	/// @param a A candidate token.
	/// @param b A candidate token.
	/// @return `true` if `a` starts before `b`, else `false`.
	static bool cmpPos(const CandidateToken *&a, const CandidateToken *&b);

	/// @brief Compare two candidate tokens by their lengths. Returns `true` if
	/// `this` is longer than `other`, else `false`.
	/// @param other Another candidate token.
	/// @return `true` if `this` is longer than `other`, else `false`.
	bool isLonger(const CandidateToken *&other) const;

	/// @brief Indicates whether two candidate tokens intersect/overlap in the
	/// text.
	/// @param other Another candidate token.
	/// @return `true` if `this` overlaps with `other`, else `false`.
	bool intersects(const CandidateToken *&other) const;

#ifndef NDEBUG
	/// @brief Get a string representation of this candidate token (debug only).
	/// @return A string representation of this candidate token.
	string toString() const;
#endif
};

/// @brief Provides restricted access to a `list<const BaseToken *>`, exposing
/// only the `getHead` and `dropHead` methods.
class TokenQueue
{
private:
	/// @brief Pointer to a list of token pointers (stored in the lexer).
	list<const BaseToken *> *data;

public:
	/// @brief Constructor.
	/// @param data Pointer to a list of token pointers (stored in the lexer).
	TokenQueue(list<const BaseToken *> *data);

	/// @brief Get the first element in the list.
	/// @return The first element in the list.
	const BaseToken *getHead() const;

	/// @brief Remove the first element from the list and return the new first
	/// element (the second element in the previous list). Return `nullptr` if
	/// the list is empty after the deletion, or if the list was empty before
	/// the method was called.
	/// @return The new first element (the second element in the previous list),
	/// or `nullptr`.
	const BaseToken *dropHead();
};

#endif