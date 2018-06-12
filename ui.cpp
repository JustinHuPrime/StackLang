#include "ui.h"
#include "stackElement.h"
#include <string>
#include <list>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
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

    for (unsigned j = 0, i = maxY - 2; i > 0 && j < s.size (); i--, j++)
    {
        move (i, 0);
        printw (*(s.front ()));
    }

    if (s.size () >= (unsigned) getmaxy (stdscr) - 2)
    {
        move (0, 0);
        clrtoeol ();
        printw ("...");
    }

    move (maxY - 1, 2 + buffer.cursorPosition ());

    refresh ();
}

void drawPrompt (const LineEditor& s)
{
    int maxY = getmaxy (stdscr);

    move (maxY - 1, 0);
    clrtoeol ();
    printw ("> ");
    move (maxY - 1, 2);
    printw (s);
    move (maxY - 1, 2 + s.cursorPosition ());

    refresh ();
}

void drawError (const string& e)
{
    int maxY = getmaxy (stdscr);

    move (maxY - 1, 0);
    clrtoeol ();
    printw (("> " + e).c_str ());
    move (maxY - 1, 2);

    refresh ();
}