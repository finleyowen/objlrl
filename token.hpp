/**
 * @file token.hpp
 *
 * @brief Declares the `TokenType` struct and the `BaseToken` class.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */

#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <string>
#include <functional>
#include <regex>
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

#endif