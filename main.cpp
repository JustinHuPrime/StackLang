#include "ui/ui.h"
#include "language/language.h"
#include "language/stackElement.h"
#include "language/stackElements/stringElement.h"
#include "language/exceptions/languageError.h"
#include "ui/lineEditor.h"
#include <ncurses.h>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
#include <map>
using namespace StackLang;
using std::string;
using std::list;
using std::ifstream;
using std::cerr;
using std::endl;
using std::map;
using StackElements::StringElement;
using StackElements::CommandElement;

const list<StackElement*>* UPDATE_STACK; //used for resize handlers - set once, then ignored
const LineEditor* UPDATE_BUFFER;

const string INFO = R"(StackLang interpreter version ALPHA 3
by Justin Hu, 2018
Use ^D to exit from prompt, and ^C to force quit
Press any key to continue...)"; //introductory stuff

const int INFOCURSX = 29; //co-ords of the end of the splash
const int INFOCURSY = 3;

namespace KeyInfo
{
    const char KEY_CTRL_D = 4; //self defined constants
}

void displayInfo () //displays info splash, they pauses
{
    move (0, 0);
    addstring (INFO.c_str ());
    move (INFOCURSY, INFOCURSX);
    getch ();
}

int main (int argc, char* argv[])
{
    int key = 0;
    list<StackElement*> s;
    map<string, list<StackElement*>> defines;
    LineEditor buffer;
    int debugmode = 0;
    bool errorFlag = false;
    ifstream fin;
    string includeFile;
    bool argsInclude = false;

    UPDATE_STACK = &s; //set const pointers for data access from event handlers
    UPDATE_BUFFER = &buffer;


    for (int i = 1; i < argc; i++) //command line args evaluation
    {
        if (string (argv[i]) == "-d") //set debug mode
        {
            if (i + 1 >= argc)
            {
                cerr << "Expected number after `-d`, found nothing. Abort." << endl;
                exit (EXIT_FAILURE);
            }

            try
            {
                debugmode = std::stoi (argv[i + 1]);
                i++;
            }
            catch (const invalid_argument& e)
            {
                cerr << "Expected number after `-d`, found `" << argv[i + 1] << "`. Abort." << endl;
                exit (EXIT_FAILURE);
            }
        }
        else if (string (argv[i]) == "-I") //start of specified files to include
        {
            argsInclude = true;
        }
        else if (string (argv[i]) == "--") //end of specified files to include
        {
            argsInclude = false;
        }
        else if (argsInclude)
        {
            s.push_front (new StringElement (argv[i]));
            s.push_front (new CommandElement ("include"));
            
            try
            {
                execute (s);
            }
            catch (const LanguageError& e)
            {

            }
        }
    }

    init ();

    displayInfo (); //splash screen

    drawPrompt (buffer); //draw the stack and prompt
    drawStack (s);

    while (true)
    {
        key = getch ();

        if (key == KeyInfo::KEY_CTRL_D) //overriding keypresses
        {
            break;
        }
        else if (errorFlag)
        {
            drawPrompt (buffer);
            drawStack (s);
            errorFlag = false;
            continue;
        }

        if (key < __SCHAR_MAX__ && isprint (key) && key != '\n' && key != '\r' && key != KEY_ENTER) //normal characters added to buffer.
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
                    s.push_front (StackElement::parse (buffer));
                    buffer.enter ();
                    drawPrompt (buffer);
                    drawStack (s);
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