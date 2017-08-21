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

struct Syntax_map :std::map<std::string, std::function<std::string(std::istream&, Syntax_map&)>> {};
// A CRTP (https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)
// Should map be hardcoded in?

using Syntax_function = std::function<std::string(std::istream&, Syntax_map&)>;

int main(int argc, char* argv[])
{
    Syntax_map syntax_table {{
        {"identifiying string",[](std::istream&,Syntax_map&) -> std::string {return "lambda expression";}}
    }};
    return 0;
}
