#include "language/exceptions/parserError.h"

namespace StackLang
{
namespace Exceptions
{

ParserError::ParserError (const string& msg, const string& ctx, unsigned loc) :
    LanguageError (msg, ctx, loc)
{}

const string ParserError::getKind () const
{
    return "Parser Error:";
}
} // namespace Exceptions
} // namespace StackLang