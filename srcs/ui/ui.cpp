#include "ui/ui.h"

#include "utils/stringUtils.h"

#include <csignal>
#include <iostream>
#include <ncurses.h>

namespace TermUI
{
using std::cerr;
using std::endl;
using Util::spaces;

void init ()
{
    setlocale (LC_ALL, "");

    initscr ();

    cbreak ();
    nonl ();
    noecho ();
    intrflush (stdscr, false);
    keypad (stdscr, true);

    atexit (uninit);

    signal (SIGWINCH, [](int dummy) {
        (void) dummy; //ignore dummy value
        endwin ();    //these commands resync ncurses with the terminal
        refresh ();

        clear (); //draw the screen
        drawStack (*UpdateStack);
        drawPrompt (*UpdateBuffer);
    });

    signal (SIGINT, [](int dummy) {
        (void) dummy; //ignore dummy value
        uninit ();
        cerr << R"(Program interrupted by terminal interrupt sequence.
Interpreter has been stopped without freeing resources
This interrupt should have been expected and intentional.)"
             << endl;
        exit (EXIT_FAILURE);
    });
}

void uninit ()
{
    endwin ();
}

void drawStack (const Stack& s)
{
    int maxY = getmaxy (stdscr);
    int offset = getcurx (stdscr);

    int prevCurMode = curs_set (CURSOR_INVISIBLE);

    for (int i = 0; i < maxY - 1; i++)
    {
        move (i, 0);
        clrtoeol ();
    }

    int i = maxY - 2;
    auto it = s.begin ();
    for (; i > 0 && it != s.end (); i--, it++)
    {
        move (i, 0);
        addstring (**it);
    }

    if (long(s.size ()) >= getmaxy (stdscr) - 2)
    {
        move (0, 0);
        clrtoeol ();
        addstring ("...");
    }

    move (maxY - 1, 2 + offset);
    curs_set (prevCurMode);

    refresh ();
}

void drawPrompt (const LineEditor& s)
{
    int maxY = getmaxy (stdscr);

    curs_set (CURSOR_INVISIBLE);
    move (maxY - 1, 0);
    clrtoeol ();
    addstring ("> ");
    addstring (s);
    move (maxY - 1, 2 + s.cursorPosition ());
    curs_set (CURSOR_VISIBLE);

    refresh ();
}

void drawError (const LanguageError& e)
{
    int centerY = getmaxy (stdscr) / 2;

    curs_set (CURSOR_INVISIBLE);
    clear ();

    if (e.hasContext ())
    {
        move (centerY - 2, 0);
        addstring (e.getKind ());
        move (centerY - 1, 0);
        addstring (e.getMessage ());
        move (centerY + 0, 0);
        addstring (e.getContext ());
        move (centerY + 1, 0);
        addstring (spaces (e.getLocation ()) + '^');
        move (centerY + 3, 0);
        addstring ("Press any key to continue...");
    }
    else
    {
        move (centerY - 1, 0);
        addstring (e.getKind ());
        move (centerY + 0, 0);
        addstring (e.getMessage ());
        move (centerY + 2, 0);
        addstring ("Press any key to continue...");
    }
}

void addstring (const string& s)
{
    unsigned char ch;
    for (const char c : s)
    {
        ch = static_cast< unsigned char > (c);
        addch (ch);
    }
}
} // namespace TermUI