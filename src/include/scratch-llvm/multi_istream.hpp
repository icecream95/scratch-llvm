#include <istream>
#include <string>

template <typename C, typename Tr = std::char_traits<C>>
class Basic_multi_streambuf :public std::basic_streambuf<C,Tr> {
public: // Thanks to https://stackoverflow.com/a/14086442
    Basic_multi_streambuf(std::basic_istream<C,Tr>& isc1, std::basic_istream<C,Tr>& isc2) :buffer{new char[1024]}, is1{&isc1}, is2{&isc2} {}
    ~Basic_multi_streambuf() { delete[] this->buffer; }
    typename Tr::int_type underflow() {
        if (this->gptr() == this->egptr()) {
            int size {0};
            if (!is1->eof()) {
                is1->read(buffer,1023);
                size = is1->gcount();
                buffer[size] = '\0';
            }
            else if (!is2->eof()) {
                is2->read(buffer,1023);
                size = is2->gcount();
                buffer[size] = '\0';
            }
            else size = 0;
            this->setg(this->buffer, this->buffer, this->buffer + size);
        }
        return (this->gptr() == this->egptr())
            ? (Tr::eof())
            : (Tr::to_int_type(*this->gptr()));
    }
private:
    char* buffer;

    std::basic_istream<C,Tr>* is1;
    std::basic_istream<C,Tr>* is2;
};

template <typename C, typename Tr = std::char_traits<C>>
struct Basic_multi_istream_base {
    Basic_multi_istream_base(std::basic_istream<C,Tr>& i1, std::basic_istream<C,Tr>& i2) :multi_istream_base_impl_buf{i1,i2} {}
    Basic_multi_streambuf<C,Tr> multi_istream_base_impl_buf;
};

template <typename C, typename Tr = std::char_traits<C>>
class Basic_multi_istream
    : virtual Basic_multi_istream_base<C,Tr>
    , public std::basic_istream<C,Tr> {
public:
    Basic_multi_istream(std::basic_istream<C,Tr>& i1, std::basic_istream<C,Tr>& i2)
        : Basic_multi_istream_base<C,Tr> {i1,i2}
        , std::basic_istream<C,Tr> {&this->multi_istream_base_impl_buf} {}
};

using Multi_istream = Basic_multi_istream<char,std::char_traits<char>>;

inline Multi_istream combine_streams(std::istream& i1, std::istream& i2)
{
    return Multi_istream{i1,i2};
}
