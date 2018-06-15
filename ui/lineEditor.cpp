#include "ui/lineEditor.h"

LineEditor::LineEditor () : preCursor (), postCursor (), preHistory (), postHistory ()
{}

void LineEditor::right ()
{
    if (postCursor.size () == 0)
    {
        beep ();
        return;
    }

    preCursor.push_back (postCursor.front ());
    postCursor.pop_front ();
}

void LineEditor::left ()
{
    if (preCursor.size () == 0)
    {
        beep ();
        return;
    }

    postCursor.push_front (preCursor.back ());
    preCursor.pop_back ();
}

void LineEditor::toEnd ()
{
    if (postCursor.size () == 0)
    {
        beep ();
        return;
    }

    while (postCursor.size () > 0)
    {
        preCursor.push_back (postCursor.front ());
        postCursor.pop_front ();
    }
}

void LineEditor::toHome ()
{
    if (preCursor.size () == 0)
    {
        beep ();
        return;
    }

    while (preCursor.size () > 0)
    {
        postCursor.push_front (preCursor.back ());
        preCursor.pop_back ();
    }
}

void LineEditor::up ()
{
    if (preHistory.size () == 0)
    {
        beep ();
        return;
    }

    postHistory.push (*this);
    
    for (auto c : preHistory.top ())
    {
        preCursor.push_back (c);
    }

    preHistory.pop ();
}

void LineEditor::down ()
{
    if (postHistory.size () == 0)
    {
        beep ();
        return;
    }

    preHistory.push (*this);

    for (auto c : postHistory.top ())
    {
        preCursor.push_back (c);
    }
}

void LineEditor::enter ()
{
    preHistory.push (*this);
    preCursor.clear ();
    postCursor.clear ();
}

void LineEditor::backspace ()
{
    if (preCursor.size () == 0)
    {
        beep ();
        return;
    }
    preCursor.pop_back ();
}

void LineEditor::del ()
{
    if (postCursor.size () == 0)
    {
        beep ();
        return;
    }
    postCursor.pop_front ();
}

int LineEditor::cursorPosition () const
{
    return preCursor.size ();
}

bool LineEditor::isEmpty () const
{
    return preCursor.empty () && postCursor.empty ();
}

void LineEditor::operator+= (char c)
{
    preCursor.push_back (c);
}

void LineEditor::operator+= (string s)
{
    for (auto c : s)
    {
        preCursor.push_back (c);
    }
}

LineEditor::operator const string () const
{
    string temp;

    for (auto c : preCursor)
    {
        temp += c;
    }

    for (auto c : postCursor)
    {
        temp += c;
    }

    return temp;
}