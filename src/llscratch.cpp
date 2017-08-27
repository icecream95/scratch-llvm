#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <limits>
#include <utility>
#include <functional>

#include <scratch-llvm/multi_istream.hpp>

inline void ignore_until(std::istream& ifs, const char ch)
{
    ifs.ignore(std::numeric_limits<std::streamsize>::max(),ch);
}

int main()
{
    return 0;
}
