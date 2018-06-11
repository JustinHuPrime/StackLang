#include "ui.h"
#include "stackElement.h"
#include "version.h"
#include <ncurses.h>
#include <string>
#include <list>
using std::string;
using std::list;

int main ()
{
    int key = 0;
    list<StackElement*> s;
    string buffer = "";

    init ();

    displayInfo ();
    getch ();

    while (true)
    {
        key = getch ();
    }
}