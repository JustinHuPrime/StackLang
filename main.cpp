#include "ui.h"
#include "stackElement.h"
#include "lineEditor.h"
#include <ncurses.h>
#include <string>
#include <list>
#include <stdexcept>
using std::string;
using std::list;
using std::invalid_argument;

const list<StackElement*>* UPDATE_STACK;
const LineEditor* UPDATE_BUFFER;

const string INFO = R"(StackLang interpreter version ALPHA 2
by Justin Hu, 2018
Press any key to continue...)";

namespace keyinfo
{
    const char Cd = 4;
}

void displayInfo ()
{
    move (0, 0);
    addstring (INFO.c_str ());
    move (2, 29);
}

int main (int argc, char* argv[])
{
    int key = 0;
    list<StackElement*> s;
    LineEditor buffer;
    int debugmode = 0;
    bool errorFlag = false;

    init ();

    UPDATE_STACK = &s;
    UPDATE_BUFFER = &buffer;

    for (int i = 1; i < argc; i++)
    {
        if (string (argv[i]) == "-d")
        {
            debugmode = std::stoi (argv[i + 1]);
        }
    }

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