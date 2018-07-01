#ifndef LANGUAGEEXCEPTION_H
#define LANGUAGEEXCEPTION_H

#include <exception>
#include <string>

namespace StackLang
{
namespace Exceptions
{
using std::exception;
using std::string;
/**
 * Describes a StackLang runtime error
 */
class LanguageException
{
  public:
    /**
     * Creates an error with a message, but no context
     */
    LanguageException (const string&);

    /**
     * Creates an error with a message and context (plus exact location of error)
     */
    LanguageException (const string&, const string&, unsigned);

    /**
     * Get error details
     */
    virtual const string getKind () const = 0;
    const string getMessage () const;
    const string getContext () const;
    unsigned getLocation () const;
    bool hasContext () const;

  protected:
    string message, context;
    unsigned location;
    bool errorHasContext;
};
} // namespace Exceptions
} // namespace StackLang

#endif /* LANGUAGEEXCEPTION_H */