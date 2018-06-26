#include "language/exceptions/languageError.h"
using namespace StackLang::Exceptions;

LanguageError::LanguageError (const string& msg) : message (msg), errorHasContext (false)
{}

LanguageError::LanguageError (const string& msg, const string& ctx, unsigned loc) : message (msg), context (ctx), location (loc), errorHasContext (true)
{}

const string LanguageError::getMessage () const
{
    return message;
}

const string LanguageError::getContext () const
{
    return context;
}

unsigned LanguageError::getLocation () const
{
    return location;
}

bool LanguageError::hasContext () const
{
    return errorHasContext;
}