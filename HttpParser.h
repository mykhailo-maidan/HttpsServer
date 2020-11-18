#ifndef _HTTP_PARSER_H_
#define _HTTP_PARSER_H_

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
    explicit HttpParser(const std::string& data);
    ~HttpParser();

    void parse();
    

private:
    std::string mDataForParsing;
};

#endif // _HTTP_PARSER_H_