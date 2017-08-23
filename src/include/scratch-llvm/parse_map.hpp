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
    std::basic_string<C,Tr,A> str() const
    {
        std::basic_string<C,Tr,A> tmp;
        for (auto x: blk)
            tmp += x.second + ' ';
        return tmp;
    }
    bool eq(const Basic_parse_string<C,Tr,A>& rhs) const
    {
        // This is faster than operator==, but doesn't do any parsing,
        //     so Parse_string{"Hello, \\w"} == "Hello, World!",
        //     but !(Parse_string{"Hello, \\w"}.eq("Hello, World!"))
        return (blk == rhs.blk)?true:false;
    }
    bool operator==(const Basic_parse_string<C,Tr,A>& rhs) const
    {
        int ind1 = 0;
        int ind2 = 0;
        for (;;) {
        }
    }
    bool operator<(const Basic_parse_string<C,Tr,A>& rhs) const
    {
        return (blk.size()<rhs.blk.size() || blk<rhs.blk)?true:false;
    }
private:
    std::vector<std::pair<bool,std::basic_string<C,Tr,A>>> blk;
};

using Parse_string = Basic_parse_string<char>;
