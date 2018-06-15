#include "ui/ui.h"
#include "language/stackElement.h"
#include <string>
#include <list>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <signal.h>
using std::list;

void init ()
{
    setlocale (LC_ALL, "");
    initscr ();
    cbreak ();
    nonl();
    noecho ();
    intrflush(stdscr, false);
    keypad(stdscr, true);
    atexit (uninit);
    signal (SIGWINCH, [] (int dummy)
    {
        endwin ();
        refresh ();
        clear ();
        drawPrompt (*UPDATE_BUFFER);
        draw (*UPDATE_STACK, *UPDATE_BUFFER);
    });
}

void drawResized (int dummy)
{
    (void) dummy;
}

void uninit ()
{
    endwin ();
}

void draw (const list<StackElement*>& s, const LineEditor& buffer)
{
    int maxY = getmaxy (stdscr);

    for (int i = 0; i < maxY - 1; i++)
    {
        move (i, 0);
        clrtoeol ();
    }

    unsigned i = maxY - 2;
    auto it = s.begin ();
    for (; i > 0 && it != s.end (); i--, it++)
    {
        move (i, 0);
        addstring (**it);
    }

    if (s.size () >= (unsigned) getmaxy (stdscr) - 2)
    {
        move (0, 0);
        clrtoeol ();
        addstring ("...");
    }

    move (maxY - 1, 2 + buffer.cursorPosition ());

    refresh ();
}

void drawPrompt (const LineEditor& s)
{
    int maxY = getmaxy (stdscr);

    move (maxY - 1, 0);
    clrtoeol ();
    addstring ("> ");
    addstring (s);
    move (maxY - 1, 2 + s.cursorPosition ());

    refresh ();
}

void drawError (const string& e)
{
    int maxY = getmaxy (stdscr);

    move (maxY - 1, 2);
    clrtoeol ();
    addstring (e.c_str ());

    refresh ();
}

void addstring (const string& s)
{
    for (const char c : s)
    {
        addch (c);
    }
}