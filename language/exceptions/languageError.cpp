#include "language/exceptions/languageError.h"
using namespace StackLang;

LanguageError::LanguageError (const string& message, const string& context, int location) : message (message), context (context), location (location)
{}

const string LanguageError::getKind () const
{
    return "language error";
}

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