#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <limits>
#include <utility>
#include <functional>

#include <scratch-llvm/istream-wrappers.hpp>

inline void ignore_until(std::istream& is, const char ch)
{
    is.ignore(std::numeric_limits<std::streamsize>::max(),ch);
}

int compile(std::istream& is, std::ostream& os)
{
    bool eof_except {!(is.exceptions()&std::ios_base::eofbit)};
    is.exceptions(is.exceptions()|std::ios_base::eofbit);
    char ch = '\0';
    try {
        while (true) {
            ch = is.get();
            if (ch == ';' || ch == '!')
            {
                ignore_until(is,'\n');
                continue;
            }
            std::string s;
            is >> s;
            static const std::set<std::string> ignore
                {"source_filename","target","attributes"};
            if (ignore.count(s))
            {
                ignore_until(is,'\n');
                continue;
           }
            if (s == "define")
                os << ""; // funct_define(is);
        }
    } catch (std::ios_base::failure&) {} // Empty body
    is.clear();
    is.exceptions(is.exceptions() ^ eof_except?std::ios_base::goodbit
                                              :std::ios_base::eofbit);
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc!=3) {
    // if (argc!=2 && argc!=3) {
        std::cerr << "Usage: " << argv[0] << " SOURCE DEST\n"
                  << "SOURCE is an LLVM IR file\n"
                  << "DEST is output to in json format\n"
                  << "To use standard input and output, use - for "
                  << "SOURCE and/or DEST\n\n";
        return 1;
    }
    if (argv[1][0]=='-') // C-style strings are zero terminated, so we
        if (argv[2][0]=='-') // can safely access the first character
            return compile(std::cin,std::cout);
        else {
            std::ofstream o{argv[2]};
            return compile(std::cin,o);
        }
    else
        if (argv[2][0]=='-') {
            std::ifstream i{argv[1]};
            return compile(i,std::cout);
        }
        else {
            std::ifstream i{argv[1]};
            std::ofstream o{argv[2]};
            return compile(i,o);
        }
}
