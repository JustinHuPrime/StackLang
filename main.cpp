#include "ui.h"
#include "stackElement.h"
#include <ncurses.h>
#include <string>
#include <list>
using std::string;
using std::list;

const string INFO = R"(StackLang interpreter version ALPHA 2
by Justin Hu, 2018
This software comes without warranty or any guarentee of fitness for any purpose.
Press any key to continue...)";

void displayInfo ()
{
    move (0, 0);
    printw (INFO.c_str ());
    move (3, 29);
}

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