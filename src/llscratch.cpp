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

struct Syntax_map :std::map<std::string,
    std::function<std::string(std::istream&, Syntax_map&)>> {};
// A CRTP
// (https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)

// Should map be hardcoded in?

using Syntax_function = std::function<
    std::string(std::istream&, Syntax_map&)>;

int main()
{
    // Syntax for syntax table identifying string:
    // Special characters are preceded by "\\".
    // The '\' character is "\\\\".
    // Alternatively, you can use raw string literals.

    // // One problem is that we need to sort the strings so we can
    // //     use map's faster search speed compared to vector etc.
    // // Just using strings won't work very well.
    // // Or should I just derive a class from map?
    // // How about specialisations? That could be a bit confusing.
    //
    // // In terms of speed, it would probably be fastest to use a
    // //     custom string type combined with a lookup function.

    // It's impossible to do what's outlined above for various reasons,
    //     and I'll probably just use a set of if statements, possibly
    //     in some sort of structure to allow re-ordering the tests
    //     easier. If you know how to make it work better, feel free
    //     to say.

    Syntax_map syntax_table {{
        {"; identifiying string",[](std::istream&, Syntax_map&)
                -> std::string
            {return "lambda expression";}},
        {";",[](std::istream& is, Syntax_map&) -> std::string
            {ignore_until(is,'\n');return "";}}
    }};
    return 0;
}
