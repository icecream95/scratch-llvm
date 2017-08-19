#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <limits>
#include <utility>
#include <functional>

using namespace std;

struct Syntax_table_function :std::function<string(istream&,map<string,Syntax_table_function>&)> {/* Empty body */}; // A CRTP (https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)
// Should map be hardcoded in?

using Syntax_map = map<string,Syntax_table_function>;

int main(int argc, char* argv[])
{
    Syntax_map syntax_table;
}
