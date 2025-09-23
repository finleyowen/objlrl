/**
 * @file token.hpp
 *
 * @brief Implements methods for the `TokenType` struct.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */

#include "token.hpp"

// constructor
TokenType::TokenType(string pat, function<BaseToken *(smatch *)> lexFn)
	: pat(pat), lexFn(lexFn) {}