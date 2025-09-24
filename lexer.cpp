/**
 * @file lexer.hpp
 *
 * @brief Implements methods for the `CandidateToken` struct and the `Lexer`
 * class.
 *
 * Copyright Finley Owen, 2025. All rights reserved.
 */
#include "lexer.hpp"

#include <sstream>
using namespace std;

// ======================
// CandidateToken methods
// ======================

// constructor
CandidateToken::CandidateToken(const TokenType *tokenType, smatch match)
    : tokenType(tokenType), match(match) {}

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

// =============
// Lexer methods
// =============

// handle unmatched input by throwing a runtime error
void Lexer::handleUnmatched(const string *s, unsigned int position,
                            unsigned int length)
{
    ostringstream ss;
    ss << "Lexer Error: unmatched input \"" << s->substr(position, length);
    ss << "\" at position " << position;
    throw runtime_error(ss.str());
}

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

// sort candidates by their starting positions
void Lexer::sortCandidates()
{
    candidates.sort(CandidateToken::cmpPos);
}

// filter out overlapping candidates
void Lexer::filterCandidates(const string *s)
{
    // need at least 2 tokens to compare them
    if (candidates.size() < 2)
        return;

    // adjacent candidates
    auto it1 = candidates.begin();
    auto it2 = next(it1);

    while (it2 != candidates.end())
    {
        // helper variables
        unsigned int end1 = (*it1)->match.position(0) + (*it1)->match.length(0);
        unsigned int start2 = (*it2)->match.position(0);

        if (end1 == start2)
        {
            // first token ends where second token starts => no overlap and no
            // unmatched input => move onto the next pair
            it1++;
            it2++;
        }
        else if (end1 < start2)
        {
            // second token starts strictly after first token ends => handle
            // unmatched input
            handleUnmatched(s, end1, start2 - end1);

            // if the program still exists after the call to handleUnmatched
            // (i.e., if the handleUnmatched method has been overriden so that
            // it doesn't throw a runtime error), move onto the next pair
            ++it1;
            ++it2;
        }
        else
        {
            // first token ends before second token starts => handle overlap
            if ((*it2)->isLonger(*it1))
            {
                /* second candidate is longer => remove first candidate. */
                delete *it1;                 // delete data in list node
                it1 = candidates.erase(it1); // delete list node
                it2 = next(it1);             // update it2
            }
            else
            {
                /* first candidate is longer OR candidates are the same length
                => remove second candidate. note that we remove the second
                candidate in the equal length case because it will be the one
                whose token type was registered with the lexer first. we know
                this because the `list::sort` method in C++ performs a stable
                sort */
                delete *it2;                 // delete data in list node
                it2 = candidates.erase(it2); // delete list node
                // don't need to update it1; it2 still points to the item after
                // it1
            }

            // don't increment the iterators here since there might be more
            // than two overlapping tokens in a row.
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

    // sort the candidate tokens
    sortCandidates();

    // filter the candidate tokens (pass `s` to generate an error message from
    // if needed)
    filterCandidates(s);

    // convert the candidate tokens to tokens; add them to the list of tokens
    for (const CandidateToken *candidate : candidates)
    {
        const BaseToken *token = candidate->tokenType->lex(&candidate->match);
        tokens.push_back(token);
    }
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

// ==================
// Debug-only methods
// ==================

// string representation of a candidate token
string CandidateToken::toString() const
{
    ostringstream ss;
    ss << tokenType->name << " token: \"" << match.str(0) << '"';
    return ss.str();
}

// string representation of the candidate tokens
string Lexer::candidatesString() const
{
    ostringstream ss;
    for (const CandidateToken *candidate : candidates)
    {
        ss << candidate->toString() << "\n";
    }
    return ss.str();
}

// string representation of the tokens lexed
string Lexer::tokensString() const
{
    ostringstream ss;
    for (const BaseToken *token : tokens)
    {
        ss << token->toString() << "\n";
    }
    return ss.str();
}

#endif

//