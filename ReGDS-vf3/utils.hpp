#include <vector>
#include <string_view>
#include <utility>
    
template < typename StringT,
           typename DelimiterT = char,
           typename ContainerT = std::vector<std::string_view> >
ContainerT split(StringT const& str, DelimiterT const& delimiters = ' ', bool trimEmpty = true, ContainerT&& tokens = {})
{
    typename StringT::size_type pos, lastPos = 0, length = str.length();

    while (lastPos < length + 1)
    {
        pos = str.find_first_of(delimiters, lastPos);
        if (pos == StringT::npos)
            pos = length;

      if (pos != lastPos || !trimEmpty)
            tokens.emplace_back(str.data() + lastPos, pos - lastPos);

        lastPos = pos + 1;
    }

    return std::forward<ContainerT>(tokens);
}

