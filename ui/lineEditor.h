#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include <string>
#include <list>
#include <stack>
#include <ncurses.h>
using std::stack;
using std::list;
using std::string;

class LineEditor
{
    public:

    /**
     * Create blank lineEditor
     */
    LineEditor ();

    /**
     * Move the cursor
     */
    void right ();
    void left ();
    void toEnd ();
    void toHome ();

    /**
     * Scroll through the command history - cursor defaults to right side.
     */
    void up ();
    void down ();

    /**
     * Add something to the history
     */
    void enter ();

    /**
     * Delete characters to the left or right of cursor
     */
    void backspace ();
    void del ();

    /**
     * Get properties of the editor - current cursor left/right, or emptiness of the editor
     */
    int cursorPosition () const;
    bool isEmpty () const;
    
    /**
     * Add a character or a string to the editor
     */
    void operator+= (char);
    void operator+= (string);

    /**
     * Produce the string
     */
    operator const string () const;

    private:

    list<char> preCursor;
    list<char> postCursor;

    stack<string> preHistory;
    stack<string> postHistory;
};

#endif /* LINEEDITOR_H */