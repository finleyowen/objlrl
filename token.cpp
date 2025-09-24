/**
 * @file token.cpp
 *
 * @brief Implements methods for the `TokenType` struct, the `BaseToken`
 * class, the `CandidateToken` struct, and the `TokenQueue` class.
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

// ======================
// CandidateToken methods
// ======================

// constructor
CandidateToken::CandidateToken(const TokenType *tokenType, const smatch match,
							   const string *src)
	: tokenType(tokenType), match(match), src(src) {}

// compare two candidate tokens by their starting positions
bool CandidateToken::cmpPos(const CandidateToken *&a, const CandidateToken *&b)
{
	return a->match.position(0) < b->match.position(0);
}

// compare two candidate tokens by length
bool CandidateToken::isLonger(const CandidateToken *&other) const
{
	return this->match.length(0) > other->match.length(0);
}

// indicate if two candidates overlap/intersect each other
bool CandidateToken::intersects(const CandidateToken *&other) const
{
	unsigned int thisStart = this->match.position(0),
				 thisLength = this->match.length(0);

	unsigned int otherStart = other->match.position(0),
				 otherLength = other->match.length(0);

	// if this starts first
	if (thisStart < otherStart)
	{
		// intersection occurs when other starts before this ends
		// in other words, the end of this occurs after the start of other
		return thisStart + thisLength > otherStart;
	}

	// if other starts first
	else if (thisStart > otherStart)
	{
		// intersection occurs when this starts before other ends
		// in other words, the end of other occurs after the start of this
		return otherStart + otherLength > thisStart;
	}
	// if the two start in the sampe place they must intersect
	return true;
}

// ==================
// TokenQueue methods
// ==================

// constructor
TokenQueue::TokenQueue(list<const BaseToken *> *data) : data(data) {}

// return the first element or null if the list is empty
const BaseToken *TokenQueue::getHead() const
{
	if (data->empty())
		return nullptr;

	return data->front();
}

// delete the first element, return new first element or null if:
// 		a) the list was already empty before the method call
// 		b) the list is empty after the method call
const BaseToken *TokenQueue::dropHead()
{
	// return null on empty
	if (data->empty())
		return nullptr;

	// delete the first data item
	delete data->front();

	// remove the first node from the list
	auto next = data->erase(data->begin());

	// decide what to return
	if (next == data->end())
		return nullptr;
	else
		return *next;
}

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

// string representation of a candidate token
string CandidateToken::toString() const
{
	ostringstream ss;
	ss << tokenType->name << " candidate: \"" << match.str(0) << '"';
	return ss.str();
}

#endif