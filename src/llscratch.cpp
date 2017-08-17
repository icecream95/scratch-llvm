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

    std::string parse_code(std::istream& ifs)
    {
        std::stringstream s;
        s << ifs.rdbuf();
        return s.str();
    }

    void ignore_until(std::istream& ifs, const char ch)
    {   // This is just a helper function to cut down on repeated code
        ifs.ignore(std::numeric_limits<std::streamsize>::max(),ch);
    }

    std::string get_block(std::istream& ifs, const char open, const char close, int depth = 0)
    {   // NOTE: this needs improvement for braces inside strings etc.
        std::string output;
        bool nlooped {true};
        char ch {' '};
        while (ifs.good() && (nlooped || depth != 0))
        {
            nlooped = false;
            while ((ch = ifs.get(), output += ch, ch!=open) && ch!=close); // I'm pretty sure there's a cleaner way to do this
            if (ch == open) ++depth;
            else            --depth;
        }
        return output;
    }

    std::string funct_define(std::istream& ifs)
    {
        std::stringstream to_parse {get_block(ifs,'{','}')};
        to_parse.exceptions(std::ios_base::eofbit);
        std::string output;
        std::string tmp;
        char ctmp;
        while (to_parse.get()!='@'); // Empty body
        std::string name;
        while ((to_parse.get(ctmp), ctmp!='(') && (name += ctmp, true));
        // The function name is in name
        std::string args = get_block(to_parse,'(',')',1);
        args = args.substr(0,args.size()-1);
        // The arguments the function takes is in args
        ignore_until(to_parse,'{');
        std::string body = get_block(to_parse,'{','}',1);
        body = body.substr(0,body.size()-1);
        // The function body is in body

        std::stringstream body_s {body};
        ignore_until(body_s,':');

        return "Class:\nName: " + name + "\nArguments: " + args + "\nBody: " + parse_code(body_s) + '\n';
    }

    std::string convert(std::istream& ifs)
    {
        ifs.exceptions(std::ios_base::eofbit);
        // Maybe we should reset exceptions when we are done
        std::string output;
        char ch = '\0';
        try {
            while (true)
            {
                // std::cerr << '.';
                ch = ifs.peek();
                if (ch == ';' || ch == '!')
                {
                    ignore_until(ifs,'\n');
                    continue;
                }
                std::string s;
                ifs >> s;
                static const std::set<std::string> ignore
                    {"source_filename","target","attributes"};
                if (ignore.count(s))
                {
                    ignore_until(ifs,'\n');
                    continue;
                }
                if (s == "define")
                    output += funct_define(ifs);
            }
        } catch (std::ios_base::failure&) {} // Empty body
        return output;
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
    return 0;
}
