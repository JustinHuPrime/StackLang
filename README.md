# StackLang Programming Language

A stack-oriented programming language with an interpreter implemented in C++. This language is heavily inspired by the DrRacket xSL teaching languages and HP's RPL.

## Getting Started

A StackLang interpreter has been implemented in C++. This interpreter is built using the C++17 standard, and compiled using g++. However, this project should be able to be built on any system. The included makefile is set up to compile the interpreter on a Linux/Unix-like system using g++. For a complete language reference, see the [documentation pages](https://techteamai.github.io/StackLang/index.html).

### Prerequisites

StackLang requires:

* ncurses
* gmpxx
* GNU-make
* g++ OR clang++ (clang++ does not support some configured warning options in the makefile)

### Building

To build ncurses, edit the makefile to specify the correct commands for removing files, making a directory, and invoking your chosen compiler. Afterwards, run `make` or `make all`. This should generate a file named `stacklang` in the same directory as the makefile.

*TODO: MOVE EVERYTHING AFTER HERE TO DOCUMENTATION PAGES*

The core of StackLang is the stack. The stack is where any and all data your program will operate on will be stored. The command line will add elements to the stack. After an element has been added to the stack, it is executed.

### Stack Elements

Stack elements can be:

* Booleans: enter either `true` or `false` on the command line. Displayed as `true` or `false`

* Commands: enter a valid command name. Permitted characters: any alphanumeric character, `-`, `?`, and `*`. Note that the command doesn't have to exist. Displayed as `<commandName>`.

* Numbers:

* Strings: enter an escaped string enclosed in unescaped double quotes. Recognized escape sequences are `\"`, `\n`, and `\\`. Displayed as an escaped string enclosed in double quotes.

* Substacks: enter `<<`, followed by a series of comma separated stack elements (whitespace after a comma is stripped when parsing), followed by `>>`. Do not include a comma before the ending delimiter. Displayed as `<<element, element, element>>`. When read from a file, the filereader will automatically build substacks: any newlines between a `<<` and the matching `>>` are converted into a comma.

* Types: enter a valid type name. Valid type names are: `Boolean`, `Command`, `Number`, `String`, `Substack`, `Type`, and `Any`.

### The Parser

The parser reads raw input from the command line and turns that input into a string. Any errors with the given input result in a ParserError. The parser error will display an error message, the line of input that caused the error, and specify the character that caused the error.

### File Inclusion

*NYI*

Stacklang files may have any file extension, but the standard file extension used by the author is `.sl`. Stacklang files are encodable in ASCII, so the interpreter must, at a minimum, be able to read ASCII characters. Support for extended character sets is entierly implementation-specific.

The file to be read is read line by line. If the line does not contain a `<<` at the start of the line, then it is passed to the command line parser, and parsed into a stack element as if that line had been entered on the command line. If the line contains a `<<` at the start of the line, then the file reader will read the next line. Then, the ending newline on the first line read is removed, a `", "` appended (unless the previous line was blank, or only contained a `<<`), and the second line appended. If the second line contains a `<<` at the start of the line, then the reader recurses to read in a substack. If the second line does not contain a `>>`, then the reader continues reading. If the second line contains a `>>` anywhere except in a string, then the lines read in so far are given to the parser or returned. A `>>` not at the end of the line will cause a ParserError when parsed.

Any line beginning with a `;` is ignored completely. The `;` comment is only recognized if it is at the start of a line.

### Execution Rules

The top item of the stack is popped, and the type of that element determined. If the top item on the stack any type other than a command, then the item is replaced onto the stack, and execution stops. If the top item is a primitive command (`plus`, `divide`, `swap`, other built-ins), then the arguments to the command are popped off of the stack, and the command executed by the interpreter. If the top item is a defined command (user definined commands, commands from included files), then the interpreter looks up the command body to go with the command name, and places those commands on the stack all at once, and will not execute commands after a single command has been put onto the stack. If execution has not been stopped by the evaluation of a data item, then the new top element of the stack is executed. If the command entered does not exist, an error is signalled.
