#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <limits>
#include <utility>

void usage(std::string pn)
{
    std::cerr << "Usage: " << pn << " SOURCE DEST\n\nSOURCE should be a LLVM IR file, and DEST is a json file\n\n";
    // TODO: // std::cerr << "Usage: " << pn << " SOURCE [DEST]\n\nIf DEST is missing, will output to SOURCE, replacing .ll with .json\nTo read/write using standard input, use - for SOURCE and/or DEST\n\n";
}

namespace Convert {

    inline std::string without_last(const std::string& s)
    {
        return s.substr(0,s.size()-1);
    }

    inline void strip_last(std::string& s)
    {
        s = without_last(s);
    }

    inline void ignore_until(std::istream& ifs, const char ch)
    {   // This is just a helper function to cut down on repeated code
        ifs.ignore(std::numeric_limits<std::streamsize>::max(),ch);
    }


    std::string parse_code(std::istream& ifs)
    {
        std::stringstream s;
        s << ifs.rdbuf();
        return s.str();
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
        strip_last(args);
        // The arguments the function takes is in args

        // Argument parser
        int argc = 0;
        std::vector<std::pair<std::string,std::pair<std::string,std::string>>> arguments;
        std::string tmpdesc;
        std::string tmptype;
        bool type_yet {false};
        std::stringstream argstream {args};
        while (argstream >> tmp)
            if (tmp[0] == '%') {
                arguments.push_back(std::make_pair(tmp.substr(1,tmp.size()-1),std::make_pair(tmptype,tmpdesc)));
                tmpdesc = "";
                tmptype = "";
                type_yet = false;
                ++argc;
            }
            else if (type_yet)
                tmpdesc += tmp + ' ';
            else {
                type_yet = true;
                tmptype = tmp;
            }
        tmp = "";
        for (auto x: arguments)
            tmp += "\"~" + name + "~" + x.second.first + "~" + x.first + "\",";

        // Scratch json for defining a function:
        // [0,0,[["procDef", "funcname %s %s %s", ["argname1", "argname2", "argname3"], ["", "", ""], true], <code here>
        output = "[0,0,[\"procDef\", \""+name+" \", [" + without_last(tmp) + "], [";
        for (int i = 0; i < argc; ++i)
            output += "\"\","; // This is where default arguments go
        strip_last(output);
        output += "], true]]]";
        // Should we store numbers as numbers, or as strings?
        // The latter would make parsing easier but a slight loss of performance may be seen. I don't think the performance difference would be too bad, as it only impacts function calls

        ignore_until(to_parse,'{');
        std::string body = get_block(to_parse,'{','}',1);
        strip_last(body);
        // The function body is in body

        std::stringstream body_s {body};
        ignore_until(body_s,':');

        return output;
        // return "Class:\nName: " + name + "\nArguments: " + args + "\nBody: " + parse_code(body_s) + '\n';
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
