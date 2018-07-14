# StackLang Programming Language

A stack-oriented programming language with an interpreter implemented in C++. This language is heavily inspired by the DrRacket xSL teaching languages and HP's RPL.

## Getting Started

A StackLang interpreter has been implemented in C++. This interpreter is built using the C++17 standard, and compiled using g++. However, this project should be able to be built on any system. The included makefile is set up to compile the interpreter on a Linux/Unix-like system using g++.

### Prerequisites

StackLang requires:

* ncurses
* gmpxx
* GNU-make
* g++ OR clang++ (clang++ does not support some configured warning options in the makefile)

### Building

To build ncurses, edit the makefile to specify the correct commands for removing files, making a directory, and invoking your chosen compiler. Afterwards, run `make` or `make all`. This should generate a file named `stacklang` in the same directory as the makefile.

## Using the Interpreter

The interpreter has several command line options:

* `-?`, `-h`: prints a summary of the command line options, then exits immediately.
* `-d N`: activates debugging of the interpreter in mode N - this is not guarenteed to have an effect. The default mode of 0 is guarenteed to have no effect.
* `-I filepath ...`: automatically includes files (at the specified path) to be read at startup. These files should only contain `define`s. Filepaths may be enclosed in quotes. Any unquoted string that starts with `-` will cause it to parse a new option. The `include` command will first look from the directory containing the `stacklang` executable for a `libraries` folder, then within that folder for the named file, the named file with a `.sl` extension, then search in the current working directory for the named file, then the named file with a `.sl` extension.
* `-l N`: limits the stack to N elements in size - interpreter will abort if attempting to put more than N elements onto the stack. Default is limited by the size of a long int on the local system.
* `-o file`: file to print the stack to (in formatted mode) when the interpreter exits via `Ctrl-D`. The active end of the stack will be the first line of the file.

### Keyboard Controls

The interpreter recognizes two control sequences:

* `Ctrl-D`: gracefully stops the interpreter as soon as it is idle. Will not interrupt currently running processes/execution.
* `Ctrl-C`: this key is terminal dependent, as it is really a raised `SIGINTR`.
  * if the interpreter is executing a command: *NYI* forcefully stops the execution.
  * if the interpreter is not executing a command: forcefully stops the interpreter. Exits without outputting the stack.
* `Ctrl-\`: this forcefully and immediately stops the interpreter. This key is terminal dependent, as it is really a raised `SIGQUIT`.

## The Language

For a complete listing of builtin and standard library commands, see the [documentation pages](link)

The core of StackLang is the stack. The stack is where any and all data your program will operate on will be stored. The command line will add elements to the stack. After an element has been added to the stack, it is executed.

### Stack Elements

Stack elements can be:

* Booleans: enter either `true` or `false` on the command line. Displayed as `true` or `false`

* Commands: enter a valid command name. Permitted characters: any alphanumeric character, `-`, `?`, and `*`. Note that the command doesn't have to exist. Displayed as `<commandName>`.

* Numbers: enter a decimal (with a `.`) or a fraction (with a `/`), but not a fraction of a decimal. Numbers may use the `'` as a thousands separator. When a number is parsed, this separator is discarded. Numbers are represented in this implementation using GMP's rationals, and as such, are inefficient but arbitrarily precise. Displayed a a fraction, without a thousands separator.

* Strings: enter an escaped string enclosed in unescaped double quotes. Recognized escape sequences are `\"`, `\n`, and `\\`. Displayed as an escaped string enclosed in double quotes.

* Substacks: *NYI* enter `<<`, followed by a series of comma separated stack elements (whitespace after a comma is stripped when parsing), followed by `>>`. Do not include a comma before the ending delimiter. Displayed as `<<element, element, element>>`. When read from a file, the filereader will automatically build substacks: any newlines between a `<<` and the matching `>>` are converted into a comma.

* Types: enter a valid type name. Valid type names are: `Boolean`, `Command`, `Number`, `String`, `Substack`, `Type`, and `Any`.

### The Parser

The parser reads raw input from the command line and turns that input into a string. Any errors with the given input result in a ParserError. The parser error will display an error message, the line of input that caused the error, and specify the character that caused the error.

### File Inclusion

*NYI*

Stacklang files may have any file extension, but the standard file extension used by the author is `.sl`. Stacklang files are encodable in ASCII, so the interpreter must, at a minimum, be able to read ASCII characters. Support for extended character sets is entierly implementation-specific.

The file to be read is read line by line. If the line does not contain a `<<` at the start of the line, then it is passed to the command line parser, and parsed into a stack element as if that line had been entered on the command line. If the line contains a `<<` at the start of the line, then the file reader will read the next line. Then, the ending newline on the first line read is removed, a `", "` appended, and the second line appended. If the second line contains a `<<` at the start of the line, then the reader recurses to read in a substack. If the second line does not contain a `>>`, then the reader continues reading. If the second line contains a `>>` anywhere except in a string, then the lines read in so far are given to the parser or returned. A `>>` not at the end of the line will cause a ParserError when parsed.

### Execution Rules

The top item of the stack is popped, and the type of that element determined. If the top item on the stack any type other than a command, then the item is replaced onto the stack, and execution stops. If the top item is a primitive command (`plus`, `divide`, `swap`, other built-ins), then the arguments to the command are popped off of the stack, and the command executed by the interpreter. If the top item is a defined command (user definined commands, commands from included files), then the interpreter looks up the command body to go with the command name, and places those commands on the stack all at once, and will not execute commands after a single command has been put onto the stack. If execution has not been stopped by the evaluation of a data item, then the new top element of the stack is executed. If the command entered does not exist, an error is signalled.
