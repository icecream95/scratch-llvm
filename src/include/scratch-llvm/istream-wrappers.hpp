#include <istream>
#include <string>
#include <array>
#include <sstream>

template<typename C, typename Tr = std::char_traits<C>>
class Basic_multi_streambuf :public std::basic_streambuf<C,Tr> {
public: // Thanks to https://stackoverflow.com/a/14086442
    Basic_multi_streambuf(std::basic_istream<C,Tr>& isc1,
                          std::basic_istream<C,Tr>& isc2)
        :is1{&isc1}, is2{&isc2} {}
    Basic_multi_streambuf(std::basic_istream<C,Tr>* isc1,
                          std::basic_istream<C,Tr>& isc2)
        :is1{&isc1}, is2{&isc2}, del1{true} {}
    Basic_multi_streambuf(std::basic_istream<C,Tr>& isc1,
                          std::basic_istream<C,Tr>* isc2)
        :is1{&isc1}, is2{&isc2}, del2{true} {}
    Basic_multi_streambuf(std::basic_istream<C,Tr>* isc1,
                          std::basic_istream<C,Tr>* isc2)
        :is1{&isc1}, is2{&isc2}, del1{true}, del2{true} {}
    ~Basic_multi_streambuf()
    {
        if (del1)
            delete is1;
        if (del2)
            delete is2;
    }
    typename Tr::int_type underflow() {
        if (this->gptr() == this->egptr()) {
            int size {0};
            if (!is1->eof()) {
                is1->read(buffer.data(),buffer.size()-1);
                size = is1->gcount();
                buffer[size] = '\0';
            }
            else if (!is2->eof()) {
                is2->read(buffer.data(),buffer.size()-1);
                size = is2->gcount();
                buffer[size] = '\0';
            }
            else size = 0;
            this->setg(this->buffer.data(), this->buffer.data(),
                       this->buffer.data() + size);
        }
        return (this->gptr() == this->egptr())
            ? (Tr::eof())
            : (Tr::to_int_type(*this->gptr()));
    }
    std::basic_istream<C,Tr>& first() {return *is1;}
    std::basic_istream<C,Tr>& second() {return *is2;}
private:
    std::array<char,1024> buffer;

    std::basic_istream<C,Tr>* is1;
    std::basic_istream<C,Tr>* is2;
    bool del1 {false};
    bool del2 {false};
};

template<typename C, typename Tr = std::char_traits<C>>
struct Basic_multi_istream_base {
    Basic_multi_istream_base(std::basic_istream<C,Tr>& i1,
                             std::basic_istream<C,Tr>& i2)
        :multi_istream_base_impl_buf{i1,i2} {}
    Basic_multi_istream_base(std::basic_istream<C,Tr>* i1,
                             std::basic_istream<C,Tr>& i2)
        :multi_istream_base_impl_buf{i1,i2} {}
    Basic_multi_istream_base(std::basic_istream<C,Tr>& i1,
                             std::basic_istream<C,Tr>* i2)
        :multi_istream_base_impl_buf{i1,i2} {}
    Basic_multi_istream_base(std::basic_istream<C,Tr>* i1,
                             std::basic_istream<C,Tr>* i2)
        :multi_istream_base_impl_buf{i1,i2} {}
    Basic_multi_streambuf<C,Tr> multi_istream_base_impl_buf;
};

template<typename C, typename Tr = std::char_traits<C>>
class Basic_multi_istream
    : virtual Basic_multi_istream_base<C,Tr>
    , public std::basic_istream<C,Tr> {
public:
    Basic_multi_istream(std::basic_istream<C,Tr>& i1,
                        std::basic_istream<C,Tr>& i2)
        : Basic_multi_istream_base<C,Tr> {i1,i2}
        , std::basic_istream<C,Tr>
              {&this->multi_istream_base_impl_buf} {}
    Basic_multi_istream(std::basic_istream<C,Tr>* i1,
                        std::basic_istream<C,Tr>& i2)
        : Basic_multi_istream_base<C,Tr> {i1,i2}
        , std::basic_istream<C,Tr>
              {&this->multi_istream_base_impl_buf} {}
    Basic_multi_istream(std::basic_istream<C,Tr>& i1,
                        std::basic_istream<C,Tr>* i2)
        : Basic_multi_istream_base<C,Tr> {i1,i2}
        , std::basic_istream<C,Tr>
              {&this->multi_istream_base_impl_buf} {}
    Basic_multi_istream(std::basic_istream<C,Tr>* i1,
                        std::basic_istream<C,Tr>* i2)
        : Basic_multi_istream_base<C,Tr> {i1,i2}
        , std::basic_istream<C,Tr>
              {&this->multi_istream_base_impl_buf} {}
    std::basic_istream<C,Tr>& first()
    {
        return this->multi_istream_base_impl_buf.first();
    }
    std::basic_istream<C,Tr>& second()
    {
        return this->multi_istream_base_impl_buf.second();
    }
};

using Multi_istream = Basic_multi_istream<char,std::char_traits<char>>;

template<typename C, typename Tr>
inline Basic_multi_istream<C,Tr> combine_streams(
    std::basic_istream<C,Tr>& i1, std::basic_istream<C,Tr>& i2)
{
    return Basic_multi_istream<C,Tr>{i1,i2};
}

template<typename C, typename Tr>
inline std::basic_istream<C,Tr>& collapse(Basic_multi_istream<C,Tr>& i)
{
    // TODO: recursive collapsing - use dynamic_cast to check if
    //       first or second are Basic_multi_
    if (i.first().eof())
        return i.second();
    if (i.second().eof())
        return i.first();
    return i;
}

template<typename C, typename Tr = std::char_traits<C>>
class Basic_shared_streambuf :public std::basic_streambuf<C,Tr> {
public:
};
