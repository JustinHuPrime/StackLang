#ifndef UI_H
#define UI_H

#include "stackElement.h"
#include "lineEditor.h"
#include <string>
#include <list>
using std::string;
using std::list;

void init ();
void uninit ();
void draw (const list<StackElement*>&, const LineEditor&);
void drawPrompt (const LineEditor&);
void drawError (const string&);
void addstring (const string& s);

extern const list<StackElement*>* UPDATE_STACK;
extern const LineEditor* UPDATE_BUFFER;

#endif /* UI_H */