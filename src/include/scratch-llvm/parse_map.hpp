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
        // Should this have an out of class equivalent?

        // This is faster than operator==, but doesn't do any parsing,
        //     so Parse_string{"Hello, \\w"} == "Hello, World!",
        //     but !(Parse_string{"Hello, \\w"}.eq("Hello, World!"))
        return (blk == rhs.blk)?true:false;
    }
    bool less(const Basic_parse_string<C,Tr,A>& rhs) const
    {
        // This is similar to eq: it is faster than operator<,
        //     but is just a shallow less and doesn't do any parsing
        return (blk.size()<rhs.blk.size() || blk<rhs.blk)?true:false;
    }
    bool operator==(const Basic_parse_string<C,Tr,A>& rhs) const
    {
        // We could optimise this by having a one-sided equals
        //     (i.e. a==b && !(b==a) could be true) but that
        //     would be confusing, and could lead to obscure errors
        int i1 = 0;
        int i2 = 0;
        while (i1!=blk.size() && i2!=rhs.blk.size()) {
            if (blk[i1] == rhs.blk[i2]) {
                ++i1;
                ++i2;
                continue;
            }
            if (blk[i1].first == rhs.blk[i2].first)
                return false;
            
        }
    }
    bool operator<(const Basic_parse_string<C,Tr,A>& rhs) const
    {
        // This needs to be modified so that
        //        (a==b) == !(a<b || b<a)
        //     holds true. Currently, it is only
        //         a.eq(b) == !(a<b || b<a)
        //     that is always true.
        // We need to do this as otherwise lookup in a map may fail.
        return (blk.size()<rhs.blk.size() || blk<rhs.blk)?true:false;
    }
private:
    std::vector<std::pair<bool,std::basic_string<C,Tr,A>>> blk;
};

using Parse_string = Basic_parse_string<char>;
