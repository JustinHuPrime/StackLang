#include "language/exceptions/parserError.h"
using namespace StackLang::Exceptions;

ParserError::ParserError (const string& message, const string& context, int location) : LanguageError (message, context, location)
{}

const string ParserError::getKind () const
{
    return "Parser Error:";
}