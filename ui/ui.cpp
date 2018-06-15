#include "ui/ui.h"

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
    {(void) dummy; //ignore dummy value
        endwin (); //these commands resync ncurses with the terminal
        refresh ();

        clear (); //draw the screen
        drawPrompt (*UPDATE_BUFFER);
        drawStack (*UPDATE_STACK);
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

void drawStack (const list<StackElement*>& s)
{
    int maxY = getmaxy (stdscr);
    int offset = getcurx (stdscr);

    int prevCurMode = curs_set (CURSOR_INVISIBLE);

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
    move (centerY - 1, 0);
    addstring (e.getKind ());
    move (centerY, 0);
    addstring (e.getMessage ());
    move (centerY + 1, 0);
    addstring (e.getContext ());
    move (centerY + 2, 0);
    addstring (spaces (e.getLocation ()) + '^');

    /* FOrmat:
    Syntax Error:/Parser Error:/Runtime Error:
    error message
    error context
    error location line 2 (generated from int)
    */
}

void addstring (const string& s)
{
    for (const char c : s)
    {
        addch (c);
    }
}