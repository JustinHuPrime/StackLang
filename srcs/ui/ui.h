#ifndef UI_H
#define UI_H

#include "language/exceptions/languageException.h"
#include "language/stack.h"
#include "language/stack/stackElement.h"
#include "language/stack/stackIterator.h"
#include "ui/lineEditor.h"
#include "utils/stringUtils.h"

#include <string>

namespace TermUI
{
using StackLang::Stack;
using StackLang::StackElement;
using StackLang::Exceptions::LanguageException;
using std::string;
using Util::LineEditor;

/**
 * initializes/uninitializes curses (and general stuff) with preset options
 */
void init ();
void uninit ();

/**
 * draws the stack, prompt, or error
 */
void drawStack (const Stack&);
void drawPrompt (const LineEditor&);
void drawError (const LanguageException&);

/**
 * adds an std::string at the current cursor position
 */
void addstring (const string& s);

/**
 * cursor constants
 */
const int CURSOR_INVISIBLE = 0;
const int CURSOR_VISIBLE = 1;
const int CURSOR_VERY_VISIBLE = 2;
} // namespace TermUI

/**
 * constants used by window resize event handler to call drawStack
 */
extern const StackLang::Stack* UpdateStack;
extern const Util::LineEditor* UpdateBuffer;

#endif /* UI_H */