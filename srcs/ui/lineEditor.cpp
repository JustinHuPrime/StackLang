#include "ui/lineEditor.h"

namespace Util
{
LineEditor::LineEditor() :
    preCursor(), postCursor(), preHistory(), postHistory()
{}

void LineEditor::right()
{
    if (postCursor.size() == 0)
    {
        beep();
        return;
    }

    preCursor.push_back(postCursor.front());
    postCursor.pop_front();
}

void LineEditor::left()
{
    if (preCursor.size() == 0)
    {
        beep();
        return;
    }

    postCursor.push_front(preCursor.back());
    preCursor.pop_back();
}

void LineEditor::toEnd()
{
    if (postCursor.size() == 0)
    {
        beep();
        return;
    }

    while (postCursor.size() > 0)
    {
        preCursor.push_back(postCursor.front());
        postCursor.pop_front();
    }
}

void LineEditor::toHome()
{
    if (preCursor.size() == 0)
    {
        beep();
        return;
    }

    while (preCursor.size() > 0)
    {
        postCursor.push_front(preCursor.back());
        preCursor.pop_back();
    }
}

void LineEditor::up()
{
    if (preHistory.size() == 0)
    {
        beep();
        return;
    }

    postHistory.push(string(*this));

    preCursor.clear();
    postCursor.clear();

    for (auto c : preHistory.top())
    {
        preCursor.push_back(c);
    }

    preHistory.pop();
}

void LineEditor::down()
{
    if (postHistory.size() == 0)
    {
        beep();
        return;
    }

    preHistory.push(string(*this));

    preCursor.clear();
    postCursor.clear();

    for (auto c : postHistory.top())
    {
        preCursor.push_back(c);
    }

    postHistory.pop();
}

void LineEditor::enter()
{
    if (!postHistory.empty()) //if we have stuff in the history
    {
        preHistory.push(string(*this)); //save the current thing (return it to its place)
        while (postHistory.size() > 1)  //put everything from before into it, but not our draft line
        {
            preHistory.push(postHistory.top());
            postHistory.pop();
        }

        postHistory.pop(); //get rid of the draft line
    }


    preHistory.push(string(*this)); //add current line
    preCursor.clear();              //and clear
    postCursor.clear();

    return;
}

void LineEditor::backspace()
{
    if (preCursor.size() == 0)
    {
        beep();
        return;
    }
    preCursor.pop_back();
}

void LineEditor::del()
{
    if (postCursor.size() == 0)
    {
        beep();
        return;
    }
    postCursor.pop_front();
}

void LineEditor::clear()
{
    preCursor.clear();
    postCursor.clear();
}

int LineEditor::cursorPosition() const
{
    return preCursor.size();
}

bool LineEditor::isEmpty() const
{
    return preCursor.empty() && postCursor.empty();
}

void LineEditor::operator+=(char c)
{
    preCursor.push_back(c);
}

void LineEditor::operator+=(string s)
{
    for (auto c : s)
    {
        preCursor.push_back(c);
    }
}

LineEditor::operator const string() const
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
} // namespace Util