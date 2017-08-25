/* Version 1: I am redoing this, and this remains here for reference

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <utility>

template <typename C, typename Tr = std::char_traits<C>,
          typename A = std::allocator<C>>
class Basic_parse_string {
public:
    using string_type = std::basic_string<C,Tr,A>;

    Basic_parse_string(const string_type& str)
    {
        std::basic_stringstream<C,Tr> stream{str};
        string_type tmp;
        while ((stream >> tmp,blk.push_back(std::make_pair(((tmp[0]=='\\')?true:false),tmp)),stream.good())); // Empty body
    }
    string_type str() const
    {
        string_type tmp;
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
            if (blk[i1].first)
                return parse_equal(blk[i1].second, rhs.blk, i2);
            else
                return parse_equal(rhs.blk[i2].second, blk, i1);
            string_type& sp = (blk[i1].first)?
                blk[i1].second:rhs.blk[i2].second;
            int& count = (blk[i1].first)?i1:i2;
            std::vector<std::pair<bool,string_type>>& vec =
                (blk[i1].first)?blk:rhs.blk;
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
    bool parse_equal(string_type& sp,
                     std::vector<std::pair<bool,string_type>>& vec,
                     int& count)
    {
        // The problem with this is that we need to take a regex
        //     style approach.
    }
    std::vector<std::pair<bool,string_type>> blk;
};

using Parse_string = Basic_parse_string<char>;

*/
