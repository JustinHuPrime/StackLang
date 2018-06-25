#include "ui/ui.h"
#include "language/language.h"
#include "language/stack.h"
#include "language/stackElement.h"
#include "language/stackElements/stringElement.h"
#include "language/exceptions/languageError.h"
#include "language/exceptions/stackOverflowError.h"
#include "ui/lineEditor.h"
#include <ncurses.h>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <map>
#include <limits>
using namespace StackLang;
using std::string;
using std::stoi;
using std::stol;
using std::list;
using std::ifstream;
using std::cerr;
using std::endl;
using std::map;
using std::numeric_limits;
using StackElements::StringElement;
using StackElements::CommandElement;
using Exceptions::LanguageError;
using Exceptions::StackOverflowError;

const Stack* UPDATE_STACK; //used for resize handlers - set once, then ignored
const LineEditor* UPDATE_BUFFER;

const string INFO = R"(StackLang interpreter version ALPHA 3
by Justin Hu, 2018
Use ^D to exit from prompt, and ^C to force quit
Press any key to continue...)"; //introductory stuff

namespace KeyInfo
{
    const char KEY_CTRL_D = 4; //self defined constants
}

void displayInfo () //displays info splash, they pauses
{
    curs_set (CURSOR_INVISIBLE);
    move (0, 0);
    addstring (INFO.c_str ());
    getch ();
}

void printError (const LanguageError& e)
{
    cerr << e.getKind () << endl;
    cerr << e.getMessage () << endl;
    if (e.hasContext ())
    {
        cerr << e.getContext () << endl;
        cerr << spaces (e.getLocation ()) << "^" << endl;
    }
}

int main (int argc, char* argv[])
{    
    Stack s;
    //map<string, SubstackElement> defines;

    int key = 0;    
    LineEditor buffer;

    int debugmode = 0;
    bool errorFlag = false;
    bool argsInclude = false;

    UPDATE_STACK = &s; //set const pointers for data access from event handlers
    UPDATE_BUFFER = &buffer;


    for (int i = 1; i < argc; i++) //command line args evaluation
    {
        if (string (argv[i]) == "--") //end of specified files to include
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
            catch (const LanguageError& e)
            {
                printError (e);
                cerr << "Aborting." << endl;
                return EXIT_FAILURE;
            }
        }
        else if (string (argv[i]) == "-d") //set debug mode
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
                cerr << "Expected number after `-d`, found `" << argv[i] << "`. Abort." << endl;
                exit (EXIT_FAILURE);
            }
        }
        else if (string (argv[i]) == "-I") //start of specified files to include
        {
            argsInclude = true;
        }
        else if (string (argv[i]) == "-l") //set limit
        {
            if (i + 1 >= argc)
            {
                cerr << "Expected number after `-l`, found nothing. Abort" << endl;
            }

            try
            {
                s.setLimit (stol (argv[++i]));
            }
            catch (const invalid_argument& e)
            {
                cerr << "Expected number after `-l`, found `" << argv[i] << "`. Abort." << endl;
            }
            catch (const StackOverflowError& e)
            {
                cerr << "Specified limit is too low to contain " << s.size () << " elements, the current size of the stack. Abort." << endl;
            }
        }
    }

    init ();

    displayInfo (); //splash screen

    drawStack (s);
    drawPrompt (buffer); //draw the stack and prompt

    while (true)
    {
        key = getch ();

        if (key == KeyInfo::KEY_CTRL_D) //overriding keypresses
        {
            break;
        }
        else if (errorFlag)
        {
            drawStack (s);
            drawPrompt (buffer);
            errorFlag = false;
            continue;
        }

        if (key < numeric_limits<char> ().max () && isprint (key) && key != '\n' && key != '\r' && key != KEY_ENTER) //normal characters added to buffer.
        {
            buffer += key;
            drawPrompt (buffer);
        }
        else if (key == '\n' || key == '\r' || key == KEY_ENTER) //enter
        {
            if (buffer.isEmpty ()) //empty buffer - execute top of stack.
            {
                execute (s);
            }
            else //nonempty buffer - push onto stack.
            {
                try
                {
                    s.push (StackElement::parse (buffer));
                    buffer.enter ();
                    drawStack (s);
                    drawPrompt (buffer);
                }
                catch (const LanguageError& e)
                {
                    drawError (e);
                    errorFlag = true;
                    buffer.enter ();
                }
            }
        }
        else if (key == KEY_BACKSPACE) // line editing.
        {
            buffer.backspace ();
            drawPrompt (buffer);
        }
        else if (key == KEY_DC)
        {
            buffer.del ();
            drawPrompt (buffer);
        }
        else if (key == KEY_UP)
        {
            buffer.up ();
            drawPrompt (buffer);
        }
        else if (key == KEY_DOWN)
        {
            buffer.down ();
            drawPrompt (buffer);
        }
        else if (key == KEY_RIGHT)
        {
            buffer.right ();
            drawPrompt (buffer);
        }
        else if (key == KEY_LEFT)
        {
            buffer.left ();
            drawPrompt (buffer);
        }
        else if (key == KEY_END)
        {
            buffer.toEnd ();
            drawPrompt (buffer);
        }
        else if (key == KEY_HOME)
        {
            buffer.toHome ();
            drawPrompt (buffer);
        }
        else //not recognized.
        {
            beep ();
        }

        if (debugmode == 1) //debug options
        {
            move (0, getmaxx (stdscr) - 3);
            clrtoeol ();
            addstring (std::to_string (key).c_str ());
            move (getmaxy (stdscr) - 1, buffer.cursorPosition () + 2);
        }
        else if (debugmode == 2)
        {
            move (0, 0);
            clrtoeol ();
            addstring ("|");
            addstring (buffer);
            addstring ("|");
            move (getmaxy (stdscr) - 1, buffer.cursorPosition () + 2);
        }
    }

    return 0;
}