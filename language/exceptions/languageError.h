#ifndef LANGUAGEERROR_H
#define LANGUAGEERROR_H

#include <exception>
#include <string>
using std::string;
using std::exception;

namespace StackLang
{
    namespace Exceptions
    {
        /**
         * Describes a StackLang runtime error
         */
        class LanguageError
        {
            public:

            /**
             * Creates an error with a message, but no context
             */
            LanguageError (const string&);

            /**
             * Creates an error with a message and context (plus exact location of error)
             */
            LanguageError (const string&, const string&, unsigned);

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
    }
}

#endif /* LANGUAGEERROR_H */