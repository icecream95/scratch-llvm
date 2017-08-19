#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <limits>
#include <utility>
#include <functional>

struct Syntax_table_function :public std::function<std::string(std::istream&,std::map<std::string,Syntax_table_function>&)> {};
// A CRTP (https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)
// Should map be hardcoded in?

using Syntax_map = std::map<std::string,Syntax_table_function>;

template <typename If, typename Is, typename T, typename Tr = std::char_traits<T>>
struct Basic_multi_istream {
};

template <typename If, typename Is>
struct Multi_istream :Basic_multi_istream<If,Is,char> {};

int main(int argc, char* argv[])
{
    Syntax_map syntax_table {
        {"identifiying string",Syntax_table_function{[](std::istream&,Syntax_map&){return std::string{"lambda expression"};}}}
    };
}
