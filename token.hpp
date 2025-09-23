/**
 * @file token.hpp
 *
 * @brief Declares the `TokenType` struct and the `Token` class.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */

#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <string>
#include <functional>
#include <regex>
using namespace std;

class BaseToken;

/// @brief Represents a type of token in the language.
struct TokenType
{
	/// @brief Regular expression (stored as a string) that matches tokens of
	/// this token type.
	const string pat;

	/// @brief Function to lex tokens of this token type.
	const function<BaseToken *(smatch *)> lexFn;

	/// @brief Constructor.
	/// @param pat Regular expression (as a string) that matches tokens of this
	/// token type.
	/// @param lexFn Function to lex tokens of this token type.
	TokenType(string pat, function<BaseToken *(smatch *)> lexFn);
};

/// @brief Represents a token in the program.
class BaseToken
{
protected:
	/// @brief Get the token type associated with this token.
	/// @return Pointer to a `TokenType`.
	/// @note Concrete subclasses need to override this pure virtual method.
	virtual const TokenType *getTokenType() = 0;
};

#endif