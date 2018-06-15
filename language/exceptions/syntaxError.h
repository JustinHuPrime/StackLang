#ifndef SYNTAXERROR_H
#define SYNTAXERROR_H

#include <exception>
#include <string>
using std::string;
using std::exception;

namespace StackLang
{
    class SyntaxError
    {
        public:

        SyntaxError (const string&);

        virtual const string& getKind () const;
        const string& getMessage () const;
        const string& getContext () const;
        const int& getLocation () const;

        private:

        string message, context;
        int location;

    };
}

#endif