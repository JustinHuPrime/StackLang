#include "claReader.h"

namespace TermUI
{

void claReader(const vector< string >& args, DefineMap& defines)
{
    /* bool argsInclude = false;
    bool argsQuiet = false;

    for (auto iter = args.begin (); iter != args.end (); iter++)
    {
    } */

    /*
    for (int i = 1; i < argc; i++) // command line args evaluation
    {
        if (string (argv[i]) == "--") // end of specified files to include
        {
            argsInclude = false;
        }
        else if (argsInclude)
        {
            s.push (new StringElement (argv[i]));
            s.push (new CommandElement ("include"));

            try
            {
                execute (s);
            }
            catch (const LanguageException& e)
            {
                printError (e);
                cerr << "Aborting." << endl;
                exit (EXIT_FAILURE);
            }
        }
        else if (string (argv[i]) == "-d") // set debug mode
        {
            if (i + 1 >= argc)
            {
                cerr << "Expected number after `-d`, found nothing. Abort." << endl;
                exit (EXIT_FAILURE);
            }

            try
            {
                debugmode = stoi (argv[++i]);
            }
            catch (const invalid_argument& e)
            {
                cerr << "Expected number after `-d`, found `" << argv[i] << "`. Abort."
                     << endl;
                exit (EXIT_FAILURE);
            }
        }
        else if (string (argv[i]) == "-I") // start of specified files to include
        {
            argsInclude = true;
        }
        else if (string (argv[i]) == "-l") // set limit
        {
            if (i + 1 >= argc)
            {
                cerr << "Expected number after `-l`, found nothing. Abort" << endl;
            }

            try
            {
                s.setLimit (stoul (argv[++i]));
            }
            catch (const invalid_argument& e)
            {
                cerr << "Expected number after `-l`, found `" << argv[i] << "`. Abort."
                     << endl;
            }
            catch (const StackOverflowError& e)
            {
                cerr << "Specified limit is too low to contain " << s.size ()
                     << " elements, the current size of the stack. Abort." << endl;
            }
        }
    }
    */
}
} // namespace TermUI