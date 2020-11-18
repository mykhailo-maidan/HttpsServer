#ifndef _HTTP_PARSER_H_
#define _HTTP_PARSER_H_

#include <string_view>
#include <string>

class HttpParser
{
public:

    enum class EMethod
    {
        eGet,
        ePost
    };

    struct SHeader
    {
        EMethod     method;
        std::string url;
        std::string version;
        std::string user_agent;
        std::string accept;
        std::string host;
    };

public:
    explicit HttpParser(std::string_view data);
    ~HttpParser();

    void parse();
    

private:
    std::string_view mDataForParsing;
};

#endif // _HTTP_PARSER_H_