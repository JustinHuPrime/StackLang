# StackLang Programming Language

A stack-oriented programming language with an interpreter implemented in C++. StackLang uses lazy execution to enable a simple set of execution rules while maintaining a usable and extensible language. This language is heavily inspired by the xSL teaching languages and HP's RPL.

# The Interpreter

A StackLang interpreter has been implemented in C++, and can be found in this repository.

## Getting Started

StackLang is built using the `c++17` standard, and compiled using `g++`. However, this project should be able to be built on any system. The included makefile is set up to compile the interpreter on a Linux/Unix system.

### Prerequisites

StackLang requires:

* Ncurses

### Using the Interpreter

To build StackLang, just run `make` from the root directory. This should generate the stacklang executable. This executable has several command line options:

* `-d N`: activates debugging of the interpreter in mode N - this is not guarenteed to have an effect. The default mode of 0 is guarenteed to disable debugging.
* `-I file1 ... --`: automatically `includes` files to be read at startup. These files must only contain `define`s. The terminal `--` is optional.
* `-l N`: *NYI* limits the stack to N elements in size - interpreter will abort if attempting to put more than N elements onto the stack. Default is limited by the size of a long int on the local system.
* `-o file`: *NYI* file to print the stack to (in formatted mode) when the interpreter exits.

# The Language

The core of StackLang is the stack. 