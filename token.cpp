/**
 * @file token.cpp
 *
 * @brief Implements methods for the `TokenType` struct.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */

#include "token.hpp"

// =================
// TokenType methods
// =================

// constructor
TokenType::TokenType(string name, string pat,
					 function<BaseToken *(smatch *)> lexFn)
	: name(name), pat(pat), lexFn(lexFn) {}

// =================
// BaseToken methods
// =================

// virtual destructor
BaseToken::~BaseToken() {}