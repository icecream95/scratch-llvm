#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>

void usage(std::string pn)
{
    std::cerr << "Usage: " << pn << " SOURCE DEST\n\nSOURCE should be a LLVM IR file, and DEST is a json file\n\n";
    // TODO: // std::cerr << "Usage: " << pn << " SOURCE [DEST]\n\nIf DEST is missing, will output to SOURCE, replacing .ll with .json\nTo read/write using standard input, use - for SOURCE and/or DEST\n\n";
}

std::string convert(std::ifstream& ifs)
{
    char ch = '\0';
    while (true)
    {
        ch = ifs.peek();
        if (ch == ';')
        {
            while (ifs.get() != '\n'); // Empty body
            continue;
        }
        std::string s;
        ifs >> s;
        static const std::set<std::string> ignore
        {"source_filename","target","attributes"};
    }
    return std::string{(std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>())};
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
    ofs << convert(ifs);
}
