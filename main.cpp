#include "ui/ui.h"
#include "language/stackElement.h"
#include "ui/lineEditor.h"
#include "language/language.h"
#include <ncurses.h>
#include <string>
#include <list>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <map>
using namespace StackLang;
using std::string;
using std::list;
using std::invalid_argument;
using std::ifstream;
using std::cerr;
using std::endl;
using std::map;

const list<StackElement*>* UPDATE_STACK;
const LineEditor* UPDATE_BUFFER;

const string INFO = R"(StackLang interpreter version ALPHA 3
by Justin Hu, 2018
Use ^D to exit from prompt, and ^C to force quit
Press any key to continue...)";

const int INFOCURSX = 29;
const int INFOCURSY = 3;

namespace keyinfo
{
    const char Cd = 4;
}

void displayInfo ()
{
    move (0, 0);
    addstring (INFO.c_str ());
    move (INFOCURSY, INFOCURSX);
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

    UPDATE_STACK = &s; //set const pointers for data access
    UPDATE_BUFFER = &buffer;


    for (int i = 1; i < argc; i++) // command evaluation
    {
        if (string (argv[i]) == "-d")
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
            catch (invalid_argument)
            {
                cerr << "Expected number after `-d`, found `" << argv[i + 1] << "`. Abort." << endl;;
                exit (EXIT_FAILURE);
            }
        }
        else if (string (argv[i]) == "-I")
        {
            argsInclude = true;
        }
        else if (string (argv[i]) == "--")
        {
            argsInclude = false;
        }
        else if (argsInclude)
        {
            fin.open (argv[i]);

            if (!fin.good ())
            {
                cerr << "Cannot open include file `" << argv[i] << "`. Abort." << endl;
                exit (EXIT_FAILURE);
            }

            string fileBuffer;

            while (!fin.eof ())
            {
                getline (fin, fileBuffer);

                if (fileBuffer.length () == 0 || fileBuffer[0] == '#')
                {
                    continue;
                }

                try
                {
                    s.push_front (StackElement::parse (fileBuffer));
                }
                catch (invalid_argument e)
                {
                    cerr << "Error while reading included file `" << argv[i] << "`\n" << e.what () << " Abort." << endl;
                    exit (EXIT_FAILURE);
                }
            }

            execute (s);
        }
    }

    init ();

    displayInfo (); //splash screen
    getch ();

    draw (s, buffer);
    drawPrompt (buffer);

    while (true)
    {
        key = getch ();

        if (key == keyinfo::Cd) //overriding keypresses
        {
            break;
        }
        else if (errorFlag)
        {
            drawPrompt (buffer);
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

            }
            else //nonempty buffer - push onto stack.
            {
                try
                {
                    s.push_front (StackElement::parse (buffer));
                    buffer.enter ();
                    draw (s, buffer);
                    drawPrompt (buffer);
                }
                catch (invalid_argument e)
                {
                    drawError (e.what ());
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