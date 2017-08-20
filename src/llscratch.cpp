#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <limits>
#include <utility>
#include <functional>

struct Syntax_map :std::map<std::string, std::function<std::string(std::istream&, Syntax_map&)>> {};
// A CRTP (https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)
// Should map be hardcoded in?

using Syntax_function = std::function<std::string(std::istream&, Syntax_map&)>;

template <typename If, typename Is, typename T, typename Tr = std::char_traits<T>>
struct Basic_multi_istream {
};

template <typename If, typename Is>
struct Multi_istream :Basic_multi_istream<If,Is,char> {};

int main(int argc, char* argv[])
{
    Syntax_map syntax_table {{
        {"identifiying string",[](std::istream&,Syntax_map&) -> std::string {return "lambda expression";}}
    }};
}
