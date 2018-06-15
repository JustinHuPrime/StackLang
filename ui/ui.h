#ifndef UI_H
#define UI_H

#include "language/stackElement.h"
#include "ui/lineEditor.h"
#include "language/exceptions/syntaxError.h"
#include "utils/stringUtils.h"
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

/**
 * initializes/uninitializes curses (and general stuff) with preset options
 */
void init ();
void uninit ();

/**
 * draws the stack, prompt, or error
 */
void drawStack (const list<StackElement*>&);
void drawPrompt (const LineEditor&);
void drawError (const SyntaxError&);

/**
 * adds an std::string at the current cursor position
 */
void addstring (const string& s);

/**
 * constants used by window resize event handler to call drawStack
 */
extern const list<StackElement*>* UPDATE_STACK;
extern const LineEditor* UPDATE_BUFFER;

/**
 * cursor constants
 */
const int CURSOR_INVISIBLE = 0;
const int CURSOR_VISIBLE = 1;
const int CURSOR_VERY_VISIBLE = 2;

#endif /* UI_H */