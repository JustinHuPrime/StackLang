#include "language/exceptions/languageError.h"
using namespace StackLang::Exceptions;

LanguageError::LanguageError (const string& message) : message (message), errorHasContext (false)
{}

LanguageError::LanguageError (const string& message, const string& context, int location) : message (message), context (context), location (location), errorHasContext (true)
{}

const string LanguageError::getMessage () const
{
    return message;
}

const string LanguageError::getContext () const
{
    return context;
}

int LanguageError::getLocation () const
{
    return location;
}

bool LanguageError::hasContext () const
{
    return errorHasContext;
}