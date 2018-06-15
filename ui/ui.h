#ifndef UI_H
#define UI_H

#include "language/stackElement.h"
#include "ui/lineEditor.h"
#include "language/exceptions/syntaxError.h"
#include <string>
#include <list>
#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <signal.h>
using std::list;
using std::string;
using StackLang::StackElement;
using StackLang::SyntaxError;

void init ();
void uninit ();
void draw (const list<StackElement*>&, const LineEditor&);
void drawPrompt (const LineEditor&);
void drawError (const SyntaxError&);
void addstring (const string& s);

extern const list<StackElement*>* UPDATE_STACK;
extern const LineEditor* UPDATE_BUFFER;

#endif /* UI_H */