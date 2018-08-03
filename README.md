# StackLang Programming Language

A stack-oriented programming language with an interpreter implemented in C++. This language is heavily inspired by the HtDP xSL teaching languages and HP's RPL.

* For a complete language reference, see the [documentation pages](https://justinhuprime.github.io/StackLang/index.html).
* For instructions on compiling the interpreter, see the file [INSTALLING](https://github.com/JustinHuPrime/StackLang/blob/dev/INSTALLING).

StackLang is a toy language intended mainly to explore the interesting (lack of) grammar in RPN languages. While prefix (Polish notation) and infix languages require many parenthesis to make order of operations unambiguous, postfix languages do not. The downside, however, is that postfix languages cannot have variadic functions. Additionally, the decision was made to avoid the use of grouping operators as much as possible. This leads to quoted commands and the need for an `eval`-like primitive. Grouping of commands together for control flow is mainly accomplished through the use of quoted commands acting as thunks.

StackLang is written by [Justin Hu](mailto:justin.hu@alumni.ubc.ca). See the [contact info](https://justinhuprime.github.io/StackLang/index.html#about) in the documentation pages.

.
.
.
.
.
.
.
.
.
.
.
.
*TODO: MOVE EVERYTHING AFTER HERE TO DOCUMENTATION PAGES*

The core of StackLang is the stack. The stack is where any and all data your program will operate on will be stored. The command line will add elements to the stack. After an element has been added to the stack, it is executed.

### Stack Elements

Stack elements can be:

* Substacks: enter `<<`, followed by a series of comma separated stack elements (whitespace after a comma is stripped when parsing), followed by `>>`. Do not include a comma before the ending delimiter. Displayed as `<<element, element, element>>`. When read from a file, the filereader will automatically build substacks: any newlines between a `<<` and the matching `>>` are converted into a comma.

* Types: enter a valid type name. Valid type names are: `Boolean`, `Command`, `Number`, `String`, `Substack`, `Type`, and `Any`.

### File Inclusion

*NYI*
Stacklang files may have any file extension, but the standard file extension used by the author is `.sta`. Stacklang files are encodable in ASCII, so the interpreter must, at a minimum, be able to read ASCII characters. Support for extended character sets is entierly implementation-specific.

The file to be read is read line by line. If the line does not contain a `<<` at the start of the line, then it is passed to the command line parser, and parsed into a stack element as if that line had been entered on the command line. If the line contains a `<<` at the start of the line, then the file reader will read the next line. Then, the ending newline on the first line read is removed, a `", "` appended (unless the previous line was blank, or only contained a `<<`), and the second line appended. If the second line contains a `<<` at the start of the line, then the reader recurses to read in a substack. If the second line does not contain a `>>`, then the reader continues reading. If the second line contains a `>>` anywhere except in a string, then the lines read in so far are given to the parser or returned. A `>>` not at the end of the line will cause a ParserError when parsed.

Any line beginning with a `;` is ignored completely. The `;` comment is only recognized if it is at the start of a line.

### Execution Rules

The top item of the stack is popped, and the type of that element determined. If the top item on the stack any type other than a command, then the item is replaced onto the stack, and execution stops. If the top item is a primitive command (`plus`, `divide`, `swap`, other built-ins), then the arguments to the command are popped off of the stack, and the command executed by the interpreter. If the top item is a defined command (user definined commands, commands from included files), then the interpreter looks up the command body to go with the command name, and places those commands on the stack all at once, and will not execute commands after a single command has been put onto the stack. If execution has not been stopped by the evaluation of a data item, then the new top element of the stack is executed. If the command entered does not exist, an error is signalled.
