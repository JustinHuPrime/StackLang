#include "language/exceptions/languageException.h"
#include "language/exceptions/stackOverflowError.h"
#include "language/language.h"
#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackElements/commandElement.h"
#include "language/stack/stackElements/stringElement.h"
#include "ui/claReader.h"
#include "ui/lineEditor.h"
#include "ui/ui.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <ncurses.h>
#include <stdexcept>
#include <string>
#include <vector>

namespace KeyInfo
{
const char KEY_CTRL_D = 4;
}

int main (int argc, char* argv[])
{
    using StackLang::DefineMap;
    using StackLang::Stack;
    using StackLang::StackElement;
    using StackLang::Exceptions::LanguageException;
    using StackLang::Exceptions::StackOverflowError;
    using StackLang::StackElements::CommandElement;
    using StackLang::StackElements::StringElement;
    using std::cerr;
    using std::endl;
    using std::invalid_argument;
    using std::map;
    using std::numeric_limits;
    using std::stoi;
    using std::stoul;
    using std::string;
    using std::vector;
    using TermUI::addstring;
    using TermUI::claReader;
    using TermUI::displayInfo;
    using TermUI::drawError;
    using TermUI::drawPrompt;
    using TermUI::drawStack;
    using TermUI::init;
    using TermUI::LineEditor;
    using TermUI::printError;

    Stack s;
    DefineMap defines;

    int key = 0;
    LineEditor buffer;

    int debugmode = 0;

    bool errorFlag = false;

    vector< string > args;

    for (int i = 1; i < argc; i++)
    {
        args.push_back (argv[i]);
    }

    claReader (args, defines);

    init ();

    displayInfo (); // splash screen

    drawStack (s);
    drawPrompt (buffer); // draw the stack and prompt

    while (true)
    {
        key = getch ();

        if (key == KeyInfo::KEY_CTRL_D) // overriding keypresses
        {
            break;
        }
        else if (key == EINTR)
        {
            endwin (); //these commands resync ncurses with the terminal
            refresh ();

            clear ();
            drawStack (s);
            drawPrompt (buffer);
        }
        else if (errorFlag) //anything on an error is ignored, but the error is cleared
        {
            drawStack (s);
            drawPrompt (buffer);
            errorFlag = false;
            continue;
        }

        if (key < numeric_limits< char > ().max () && isprint (key) && key != '\n' &&
            key != '\r' && key != KEY_ENTER) // normal characters added to buffer.
        {
            buffer += key;
            drawPrompt (buffer);
        }
        else if (key == '\n' || key == '\r' || key == KEY_ENTER) // enter - add and execute
        {
            try
            {
                s.push (StackElement::parse (buffer));
                buffer.enter ();
                drawStack (s);
                drawPrompt (buffer);
                execute (s, defines);
            }
            catch (const LanguageException& e)
            {
                drawError (e);
                errorFlag = true;
                buffer.clear ();
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
        else // not recognized.
        {
            beep ();
        }

        if (debugmode == 1) // debug options - must not include 0
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

    exit (EXIT_SUCCESS);
}