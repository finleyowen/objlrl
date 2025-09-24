/**
 * @file token.cpp
 *
 * @brief Implements methods for the `TokenType` struct and the `BaseToken`
 * class.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */

#include "token.hpp"

// =================
// TokenType methods
// =================

// constructor
TokenType::TokenType(string name, string pat,
					 function<const BaseToken *(const smatch *)> lexFn)
	: name(name), pat(pat), lexFn(lexFn) {}

// lex convenience method
const BaseToken *TokenType::lex(const smatch *match) const
{
	return lexFn(match);
}

// =================
// BaseToken methods
// =================

// virtual destructor
BaseToken::~BaseToken() {}

#ifndef NDEBUG

// only needed in debug mode
#include <sstream>
using namespace std;

// ==================
// Debug-only methods
// ==================

// string representation of a token type
string TokenType::toString() const
{
	return name;
}

// string representation of a token
string BaseToken::toString() const
{
	ostringstream ss;
	ss << getTokenType()->toString() << " token";
	return ss.str();
}

#endif