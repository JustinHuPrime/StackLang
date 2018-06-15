#ifndef LANGUAGEERROR_H
#define LANGUAGEERROR_H

#include <exception>
#include <string>
using std::string;
using std::exception;

namespace StackLang
{
    class LanguageError
    {
        public:

        LanguageError (const string&, const string&, int);

        virtual const string getKind () const;
        const string getMessage () const;
        const string getContext () const;
        int getLocation () const;

        private:

        string message, context;
        int location;

    };
}

#endif /* LANGUAGEERROR_H */