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
	const function<BaseToken *(smatch *)> lexFn;

	/// @brief Constructor.
	/// @param name Name for this token type.
	/// @param pat Regular expression (as a string) that matches tokens of this
	/// token type.
	/// @param lexFn Function to lex tokens of this token type.
	TokenType(string name, string pat, function<BaseToken *(smatch *)> lexFn);
};

/// @brief Represents a token in the program.
class BaseToken
{
protected:
	/// @brief Get the token type associated with this token.
	/// @return Pointer to a `TokenType`.
	/// @note Concrete subclasses need to override this pure virtual method.
	virtual const TokenType *getTokenType() = 0;

public:
	/// @brief Virtual destructor.
	virtual ~BaseToken();
};

#endif