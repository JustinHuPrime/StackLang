# StackLang Programming Language

A stack-oriented programming language with an interpreter implemented in C++. This language is heavily inspired by the HtDP xSL teaching languages and HP's RPL.

* For a complete language reference, see the [documentation pages](https://justinhuprime.github.io/StackLang/index.html).
* For instructions on compiling the interpreter, see the file [INSTALLING](https://github.com/JustinHuPrime/StackLang/blob/dev/INSTALLING).

StackLang is a toy language intended mainly to explore the interesting (lack of) grammar in RPN languages. While prefix (Polish notation) and infix languages require many parenthesis to make order of operations unambiguous, postfix languages do not. The downside, however, is that postfix languages cannot have variadic functions. Additionally, the decision was made to avoid the use of grouping operators as much as possible. This leads to quoted commands and the need for an `eval`-like primitive. Grouping of commands together for control flow is mainly accomplished through the use of quoted, locally defined, commands acting as thunks.

StackLang is written by [Justin Hu](mailto:justin.hu@alumni.ubc.ca). See the [contact info](https://justinhuprime.github.io/StackLang/index.html#about) in the documentation pages.

Additional contributions by Bronwyn Damm, Jacques Marais, Ramon Rakow, and Jude Sidloski