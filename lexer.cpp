/**
 * @file token.cpp
 *
 * @brief Implements methods for the `CandidateToken` struct and the `Lexer`
 * class.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */

#include "lexer.hpp"

// only need <sstream> in debug mode
#ifndef NDEBUG
#include <sstream>
using namespace std;
#endif

// ======================
// CandidateToken methods
// ======================

// constructor
CandidateToken::CandidateToken(const TokenType *tokenType, smatch match)
	: tokenType(tokenType), match(match) {}

// compare two candidate tokens by their starting positions
bool CandidateToken::cmpPos(CandidateToken *&a, CandidateToken *&b)
{
	return a->match.position(0) < b->match.position(0);
}

#ifndef NDEBUG
// string representation of candidate token
string CandidateToken::to_string() const
{
	ostringstream ss;
	ss << tokenType->name << " \"" << match.str(0) << '"';
	return ss.str();
}
#endif

// =============
// Lexer methods
// =============

// register a token type with the lexer
void Lexer::registerTokenType(const TokenType *tokenType)
{
	tokenTypes.push_back(tokenType);
}

// find candidate tokens for a string and add them to the candidates list
void Lexer::findCandidates(const string *s)
{
	for (const TokenType *tokenType : tokenTypes)
	{
		regex pat = regex(tokenType->pat);
		sregex_iterator it(s->begin(), s->end(), pat);
		sregex_iterator end;

		for (; it != end; it++)
		{
			CandidateToken *candidate = new CandidateToken(tokenType, *it);
			candidates.push_back(candidate);
		}
	}
}

// lex a string
void Lexer::lex(string _s)
{
	/* note that "_s" stores the original string which may be stack or heap
	allocated, while "s" stores a pointer to a heap-allocated copy of the
	original string. the lexer takes ownership of the heap allocated copy by
	adding it to the "stringsLexed" list. */
	const string *const s = new string(_s);
	stringsLexed.push_back(s);

	// find the candidate tokens for s and add them to the candidates list
	findCandidates(s);
}

// destructor
Lexer::~Lexer()
{
	// free strings lexed
	for (const string *s : stringsLexed)
	{
		delete s;
	}

	// free candidate tokens
	for (const CandidateToken *candidate : candidates)
	{
		delete candidate;
	}

	// free tokens
	for (const BaseToken *token : tokens)
	{
		delete token;
	}

	// note: the token type pointers don't need to be freed since they should
	// point to static attributes in the corresponding token classes.
}

#ifndef NDEBUG
// string representation of candidate tokens
string Lexer::candidatesStr() const
{
	ostringstream ss;
	for (const CandidateToken *candidate : candidates)
	{
		ss << candidate->to_string() << "; ";
	}
	return ss.str();
}
#endif

//