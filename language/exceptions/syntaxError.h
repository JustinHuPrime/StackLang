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

        private:

    };
}

#endif