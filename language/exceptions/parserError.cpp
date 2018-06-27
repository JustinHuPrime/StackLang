#include "language/exceptions/parserError.h"
using namespace StackLang::Exceptions;

ParserError::ParserError (const string& msg, const string& ctx, unsigned loc) :
    LanguageError (msg, ctx, loc)
{}

const string ParserError::getKind () const
{
    return "Parser Error:";
}