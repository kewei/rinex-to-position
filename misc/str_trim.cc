#include <algorithm>
#include <cctype>
#include <string>
//#include "str_trim.h"

inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {return !std::isspace(ch);}));
}

inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {return !std::isspace(ch);}).base(), s.end());
}

inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

/* void to_kml() 
{

} */