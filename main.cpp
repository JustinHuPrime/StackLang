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

const string INFO = R"(StackLang interpreter version ALPHA 2
by Justin Hu, 2018
Press any key to continue...)";

namespace keyinfo
{
    const int Cd = 4;
}

void displayInfo ()
{
    move (0, 0);
    printw (INFO.c_str ());
    move (2, 29);
}

int main ()
{
    int key = 0;
    list<StackElement*> s;
    LineEditor buffer;

    init ();

    displayInfo ();
    getch ();

    draw (s, buffer);
    drawPrompt (buffer);

    while (true)
    {
        key = getch ();

        if (isprint (key) && key != '\n' && key != '\r' && key != KEY_ENTER)
        {
            buffer += key;
            drawPrompt (buffer);
        }
        else if (!buffer.isEmpty () && (key == '\n' || key == '\r' || key == KEY_ENTER))
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
        else if (key == KEY_BACKSPACE)
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
        else if (key == keyinfo::Cd)
        {
            break;
        }
        else
        {
            beep ();
        }

        move (0, getmaxx (stdscr) - 3);
        clrtoeol ();
        printw (std::to_string (key).c_str ());
        drawPrompt (buffer);
    }

    return 0;
}