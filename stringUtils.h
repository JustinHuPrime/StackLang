#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <utility>
using std::string;
using std::pair;

bool starts_with (string, string);
bool ends_with (string, string);

string escape (string);
string unescape (string);

#endif /* STRINGUTILS_H */