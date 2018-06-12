#include "stringUtils.h"
#include <string>
using std::string;

bool starts_with (string s, string s1)
{
    return 0 == s.find (s1);
}

bool ends_with (string s, string s1)
{
    return s.length () - s1.length () == s.find (s1);
}