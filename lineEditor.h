#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include <string>
#include <list>
#include <stack>
using std::stack;
using std::list;
using std::string;

class LineEditor
{
    public:

    LineEditor ();

    void right ();
    void left ();
    void up ();
    void down ();
    void enter ();
    void backspace ();
    void del ();
    int cursorPosition () const;
    bool isEmpty () const;
    
    void operator+= (char);
    void operator+= (string);
    void operator-- ();
    void operator-- (int);
    operator const char* () const;
    operator const string () const;

    private:

    list<char> preCursor;
    list<char> postCursor;

    stack<string> preHistory;
    stack<string> postHistory;
};

#endif /* LINEEDITOR_H */