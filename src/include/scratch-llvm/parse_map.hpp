#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <utility>

template <typename C, typename Tr = std::char_traits<C>,
          typename A = std::allocator<C>>
class Basic_parse_string {
public:
    Basic_parse_string(const std::basic_string<C,Tr,A>& str)
    {
        std::basic_stringstream<C,Tr> stream{str};
        std::basic_string<C,Tr,A> tmp;
        while ((stream >> tmp,blk.push_back(std::make_pair(((tmp[0]=='\\')?true:false),tmp)),stream.good())); // Empty body
    }
    std::basic_string<C,Tr,A> str()
    {
        std::basic_string<C,Tr,A> tmp;
        for (auto x: blk)
            tmp += x.second + ' ';
        return tmp;
    }
private:
    std::vector<std::pair<bool,std::basic_string<C,Tr,A>>> blk;
};

using Parse_string = Basic_parse_string<char>;
