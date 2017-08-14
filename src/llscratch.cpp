#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <limits>

void usage(std::string pn)
{
    std::cerr << "Usage: " << pn << " SOURCE DEST\n\nSOURCE should be a LLVM IR file, and DEST is a json file\n\n";
    // TODO: // std::cerr << "Usage: " << pn << " SOURCE [DEST]\n\nIf DEST is missing, will output to SOURCE, replacing .ll with .json\nTo read/write using standard input, use - for SOURCE and/or DEST\n\n";
}

namespace Convert {
    std::string funct_define(std::istream& ifs)
    {
        return "";
    }

    std::string convert(std::istream& ifs)
    {
        static const std::string start_boilerplate
            {""}; // TODO
        std::string output {start_boilerplate};
        char ch = '\0';
        while (true)
        {
            ch = ifs.peek();
            if (ch == ';' || ch == '!')
            {
                ifs.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                continue;
            }
            std::string s;
            ifs >> s;
             static const std::set<std::string> ignore
                {"source_filename","target","attributes"};
            if (ignore.count(s))
            {
                ifs.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                continue;
            }
            if (s == "define")
                output += funct_define(ifs);
        }
        static const std::string end_boilerplate
            {""}; // TODO
        output += end_boilerplate;
        return output;
        // return std::string{(std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>())};
    }
}

int main(int argc, char* argv[])
{
    if (argc!=3) {
    // if (argc<2 || argc>3) {
        usage(std::string{argv[0]});
        return 1;
    }
    std::ifstream ifs {argv[1]};
    std::ofstream ofs {argv[2]};
    ofs << Convert::convert(ifs);
}
