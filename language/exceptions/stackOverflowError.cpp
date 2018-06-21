#include "language/exceptions/stackOverflowError.h"
using namespace StackLang::Exceptions;
using std::to_string;

StackOverflowError::StackOverflowError (int limit) : LanguageError ("Stack has exceeded configured limit of " + to_string (limit) + ".")
{}

const string StackOverflowError::getKind () const
{
    return "Stack overflowed:";
}