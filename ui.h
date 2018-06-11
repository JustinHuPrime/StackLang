#ifndef UI_H
#define UI_H

#include "stackElement.h"
#include <string>
#include <list>
using std::string;
using std::list;

void init ();
void uninit ();
void draw (const list<StackElement*>&, const string&);
void drawPrompt (const string&);
void drawError (const string&);

#endif /* UI_H */